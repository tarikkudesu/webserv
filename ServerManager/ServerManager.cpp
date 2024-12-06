#include "ServerManager.hpp"

ServerManager::ServerManager( ) :
__sockNum( 0 ),
__configFile( "default_path.conf" )
{
	std::cout << "config file " << __configFile << "\n";
}

ServerManager::ServerManager( const String &configutation_file ) :
__sockNum( 0 ),
__configFile( configutation_file )
{
	std::cout << "config file " << __configFile << "\n";
}

ServerManager::ServerManager( const ServerManager &copy )
{
	*this = copy;
}

ServerManager	&ServerManager::operator=( const ServerManager &assign )
{
	if (this != &assign) {
		this->__configFile = assign.__configFile;
		this->__sockets = assign.__sockets;
		this->__servers = assign.__servers;
		this->__sockNum = assign.__sockNum;
	}
	return *this;
}

ServerManager::~ServerManager( )
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++) {
		close(it->fd);
	}
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void	ServerManager::removeConnection( int sd )
{
	t_Connections::iterator	it = this->__connections.find(sd);
	if (it != this->__connections.end()) {
		Connection	*instance = it->second;
		this->__connections.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void	ServerManager::addConnection( int sd )
{
	this->__connections[sd] = new Connection( sd );
	addSocket(sd, CONNECTION);
}
void	ServerManager::removeServer( int sd )
{
	t_Server::iterator	it = this->__servers.find(sd);
	if (it != this->__servers.end()) {
		Server	*instance = it->second;
		this->__servers.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void	ServerManager::addServer( Server *server )
{
	this->__servers[server->getServerSocket()] = server;
	addSocket(server->getServerSocket(), SERVER);
}
void	ServerManager::removeSocket( int sd )
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++) {
		if (sd == it->fd) {
			this->__sockets.erase(it);
			this->__sockNum--;
			close(sd);
			return ;
		}
	}
}
void	ServerManager::addSocket( int sd, t_endian endian )
{
	struct pollfd	sockStruct;

	sockStruct.fd = sd;
	if (endian == SERVER)
		sockStruct.events = POLLIN;
	else if (endian == CONNECTION)
		sockStruct.events = POLLIN | POLLOUT | POLLHUP;
	// Server::setNonBlockingMode( sd );
	this->__sockets.push_back( sockStruct );
	this->__sockNum++;
}
bool	ServerManager::isServerSocket( int sd )
{
	if (this->__servers.find(sd) != this->__servers.end())
		return true;
	return false;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void	ServerManager::writeDataToSocket( int sd )
{
	/****************************************
	 * CHECK CONNECTION QUEUE FOR RESPONCES *
	 ****************************************/
	/**********************************************************
	 * DECIDE WITHER TO CLOSE THE CONNECTION OR KEEP IT ALIVE *
	 **********************************************************/
	if (this->__connections[sd]->__responseQueue.empty())
		return ;
	String	response = this->__connections[sd]->__responseQueue.front();
	this->__connections[sd]->__responseQueue.pop();
	ssize_t	bytesWritten = send(sd, response.c_str(), strlen(response.c_str()), 0);
	if (bytesWritten > 0) {
		Logs::tout("write");
	} else {
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR, \
			&sockErr, sizeof(sockErr)) == 0 && sockErr == 0) {
			return ;
		} else {
			removeConnection(sd);
		}
	}
}
void	ServerManager::readDataFromSocket( int sd )
{
	char			buff[READ_SIZE + 1];

	ssize_t bytesRead = recv(sd, buff, READ_SIZE, 0);
	if (bytesRead == 0) {
		removeConnection(sd);
	} else if (bytesRead > 0) {
		buff[bytesRead] = '\0';
		t_Connections::iterator	iter = this->__connections.find(sd);
		if (iter != this->__connections.end())
			iter->second->proccessData( String(buff) );
	} else {
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR, \
			&sockErr, sizeof(sockErr)) == 0 && sockErr == 0) {
			return ;
		} else {
			removeConnection(sd);
		}
	}
}
void	ServerManager::acceptNewConnection( int sd )
{
	int		newSock;

	newSock = accept(sd, NULL, NULL);
	if (newSock >= 0) {
		addConnection(newSock);
		Logs::tout("accept");
	} else {
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR, \
			&sockErr, sizeof(sockErr)) == 0 && sockErr == 0) {
			return ;
		} else {
			removeServer(sd);
		}
	}
}
void	ServerManager::proccessPollEvent( int retV )
{
	for (int sd = 0; sd < this->__sockNum && retV; sd++)
	{
		struct pollfd &sockStruct = this->__sockets.at(sd);
		try {
			if (sockStruct.revents & POLLIN) {
				if (isServerSocket(sockStruct.fd)) {
					acceptNewConnection(sockStruct.fd);
					retV--;
				} else {
					readDataFromSocket(sockStruct.fd);
					retV--;
				}
			} else if (sockStruct.revents & POLLOUT) {
				writeDataToSocket(sockStruct.fd);
				retV--;
			} else if (sockStruct.revents & POLLHUP) {
				removeConnection(sockStruct.fd);
				retV--;
			}
		} catch ( std::exception &e ) {
			Logs::terr( e.what() );
		}
	}
}
void	ServerManager::mainLoop()
{
	int			retV = 0;
	int			timeout = 1000;

	try {
		while (true)
		{
			retV = poll(this->__sockets.data(), this->__sockets.size(), timeout);
			if (retV == -1)
				throw std::runtime_error("poll syscall err");
			else if (retV == 0) {
				continue ;
			} else {
				this->proccessPollEvent( retV );
			}
		}
	} catch ( std::exception &e ) {
		Logs::terr( e.what() );
	}
}
void	ServerManager::setUpWebserv()
{
	/*******************************************************************************
	 *							  CREATE A CONF OBJECT, 						   *
	 *					 READ AND PARSE THE CONFIGURATION FILE, 				   *
	 *				SET UP EACH SERVER WITH ITS PORT, DOMAIN NAME ...			   *
	 * EACH SERVER MUST HAVE A LOCACTION CLASS CONFIGURED FROM THE LOCATION BLOCKS *
	 *                              HAKAROUUU, KHDMTK                              *
	 *******************************************************************************/
	{
		try {
			Server	*server = new Server("domain1.com", 443);
			server->setup();
			addServer( server );
		} catch ( std::exception &e ) { Logs::terr( e.what() ); }

		try {
			Server	*server = new Server("domain2.com", 444);
			server->setup();
			addServer( server );
		} catch ( std::exception &e ) { Logs::terr( e.what() ); }

		try {
			Server	*server = new Server("domain3.com", 445);
			server->setup();
			addServer( server );
		} catch ( std::exception &e ) { Logs::terr( e.what() ); }
	}
}
