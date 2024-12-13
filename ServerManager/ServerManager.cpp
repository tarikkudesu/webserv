#include "ServerManager.hpp"

void printLocalAddress(int sockfd) {
    struct sockaddr_in localAddr;
    socklen_t addrLen = sizeof(localAddr);

    if (getsockname(sockfd, (struct sockaddr*)&localAddr, &addrLen) == 0) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(localAddr.sin_addr), ip, sizeof(ip));
        std::cout << "Request came to IP: " << ip 
                  << ", Port: " << ntohs(localAddr.sin_port) << std::endl;
    } else {
        std::cerr << "Error: Could not retrieve local address." << std::endl;
    }
}

t_Server				ServerManager::__servers;
t_events				ServerManager::__sockets;
t_Connections			ServerManager::__connections;
int						ServerManager::__sockNum = 0;

ServerManager::ServerManager()
{

}

ServerManager::ServerManager(const String &configutation_file)
{
	WSU::l1(); WSU::l1("configuration file:" + configutation_file); WSU::l1("\n");

	try {
		Config	config(configutation_file);
		config.setupEverything();
		setUpWebserv();
		mainLoop();
	} catch ( std::exception &e ) {
		WSU::terr(e.what());
	}
}

ServerManager::ServerManager(const ServerManager &copy)
{
	*this = copy;
}

ServerManager &ServerManager::operator=(const ServerManager &assign)
{
	if (this != &assign)
	{
		ServerManager::__sockets = assign.__sockets;
		ServerManager::__servers = assign.ServerManager::__servers;
		ServerManager::__sockNum = assign.__sockNum;
	}
	return *this;
}

ServerManager::~ServerManager()
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++)
		close(it->fd);
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void ServerManager::removeConnection(int sd)
{
	t_Connections::iterator it = ServerManager::__connections.find(sd);
	if (it != ServerManager::__connections.end())
	{
		Connection *instance = it->second;
		ServerManager::__connections.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void ServerManager::addConnection(int sd)
{
	ServerManager::__connections[sd] = new Connection(sd);
	ServerManager::__connections[sd]->setServers(ServerManager::__servers);
	addSocket(sd, CONNECTION);
}
void ServerManager::removeServer(int sd)
{
	t_Server::iterator it = ServerManager::__servers.find(sd);
	if (it != ServerManager::__servers.end())
	{
		Server *instance = it->second;
		ServerManager::__servers.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void ServerManager::addServer(Server *server)
{
	ServerManager::__servers[server->getServerSocket()] = server;
	addSocket(server->getServerSocket(), SERVER);
}
void ServerManager::removeSocket(int sd)
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++)
	{
		if (sd == it->fd)
		{
			ServerManager::__sockets.erase(it);
			ServerManager::__sockNum--;
			close(sd);
			return;
		}
	}
}
void ServerManager::addSocket(int sd, t_endian endian)
{
	struct pollfd sockStruct;

	sockStruct.fd = sd;
	if (endian == SERVER)
		sockStruct.events = POLLIN;
	else if (endian == CONNECTION)
		sockStruct.events = POLLIN | POLLOUT | POLLHUP;
	Server::setNonBlockingMode( sd );
	ServerManager::__sockets.push_back(sockStruct);
	ServerManager::__sockNum++;
}
bool ServerManager::isServerSocket(int sd)
{
	if (ServerManager::__servers.find(sd) != ServerManager::__servers.end())
		return true;
	return false;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void ServerManager::writeDataToSocket(int sd)
{
	/****************************************
	 * CHECK CONNECTION QUEUE FOR RESPONCES *
	 ****************************************/
	/**********************************************************
	 * DECIDE WITHER TO CLOSE THE CONNECTION OR KEEP IT ALIVE *
	 **********************************************************/
	if (ServerManager::__connections[sd]->__responseQueue.empty())
		return;
	String response = ServerManager::__connections[sd]->__responseQueue.front();
	ServerManager::__connections[sd]->__responseQueue.pop();
	ssize_t bytesWritten = send(sd, response.c_str(), strlen(response.c_str()), 0);
	if (bytesWritten > 0)
	{
		WSU::l1(); WSU::l1("send"); WSU::l1(sd); WSU::l1("\n");
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			WSU::l1(); WSU::l1("remove"); WSU::l1(sd); WSU::l1("\n");
			removeConnection(sd);
		}
	}
}
void ServerManager::readDataFromSocket(int sd)
{
	char buff[READ_SIZE + 1];

	ssize_t bytesRead = recv(sd, buff, READ_SIZE, 0);
	if (bytesRead == 0)
	{
		removeConnection(sd);
	}
	else if (bytesRead > 0)
	{
		buff[bytesRead] = '\0';
		t_Connections::iterator iter = ServerManager::__connections.find(sd);
		if (iter != ServerManager::__connections.end())
		{
			WSU::l1(); WSU::l1("recv"); WSU::l1(sd); WSU::l1("\n");
			iter->second->proccessData(String(buff));
		}
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			WSU::l1(); WSU::l1("remove"); WSU::l1(sd); WSU::l1("\n");
			removeConnection(sd);
		}
	}
}
void ServerManager::acceptNewConnection(int sd)
{
	int newSock;

	newSock = accept(sd, NULL, NULL);
	if (newSock >= 0)
	{
		WSU::l1(); WSU::l1("accept"); WSU::l1(sd); WSU::l1("\n");
		addConnection(newSock);
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			removeServer(sd);
		}
	}
}
void ServerManager::proccessPollEvent(int retV)
{
	for (int sd = 0; sd < ServerManager::__sockNum && retV; sd++)
	{
		struct pollfd &sockStruct = ServerManager::__sockets.at(sd);
		try
		{
			if (sockStruct.revents & POLLIN)
			{
				if (isServerSocket(sockStruct.fd))
				{
					acceptNewConnection(sockStruct.fd);
					retV--;
				}
				else
				{
					readDataFromSocket(sockStruct.fd);
					retV--;
				}
			}
			else if (sockStruct.revents & POLLOUT)
			{
				writeDataToSocket(sockStruct.fd);
				retV--;
			}
			else if (sockStruct.revents & POLLHUP)
			{
				removeConnection(sockStruct.fd);
				retV--;
			}
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}
	}
}
void ServerManager::mainLoop()
{
	int retV = 0;
	int timeout = 1000;

	try
	{
		while (true)
		{
			retV = poll(ServerManager::__sockets.data(), ServerManager::__sockets.size(), timeout);
			if (retV == -1)
				throw std::runtime_error("poll syscall err");
			else if (retV == 0)
			{
				continue;
			}
			else
			{
				this->proccessPollEvent(retV);
			}
		}
	}
	catch (std::exception &e)
	{
		WSU::terr(e.what());
	}
}
void ServerManager::setUpWebserv()
{
	{
		try
		{
			Server *server = new Server("domain1.com", 1024);
			ServerManager::addServer(server);
			server->setup();
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}
		try
		{
			Server *server = new Server("domain4.com", 1024);
			ServerManager::addServer(server);
			server->setup();
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}
		try
		{
			Server *server = new Server("domain5.com", 1024);
			ServerManager::addServer(server);
			server->setup();
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}
		try
		{
			Server *server = new Server("domain2.com", 1025);
			ServerManager::addServer(server);
			server->setup();
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}

		try
		{
			Server *server = new Server("domain3.com", 1026);
			ServerManager::addServer(server);
			server->setup();
		}
		catch (std::exception &e)
		{
			WSU::terr(e.what());
		}
	}
}
