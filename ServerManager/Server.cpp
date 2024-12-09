#include "Server.hpp"

Server::Server( )
{

}

Server::Server( String serverName, int port ) :
__sd( -1 ),
__port( port ),
__serverName( serverName ),
__serverDown( false )
{

}

Server::Server( const Server &copy ) :
__sd( copy.__sd ),
__port( copy.__port ),
__serverName( copy.__serverName ),
__serverDown( copy.__serverDown )
{
}

Server::~Server()
{

}

Server	&Server::operator=( const Server &assign )
{
	if (this != &assign) {
		__sd = assign.__sd;
		__port = assign.__port;
		__serverName = assign.__serverName;
		__serverDown = assign.__serverDown;
	}	
	return *this;
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void	Server::stopServer()
{
	this->__serverDown = true;
	this->__sd = -1;
}
int		Server::getServerPort() const
{
	return	this->__port;
}
bool	Server::getServerStat() const
{
	return	this->__serverDown;
}
String	Server::getServerIP() const
{
	return this->__serverIP;
}
String	Server::getServerName() const
{
	return this->__serverName;
}
int		Server::getServerSocket() const
{
	return this->__sd;
}
/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/

void	Server::setup()
{
	struct sockaddr_in	addr;
	int					reuse = 1;

	this->__sd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->__sd == -1)
		throw std::runtime_error( this->__serverName + ": socket syscall, failed to create socket" );
	if (-1 == setsockopt(__sd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)))
		throw std::runtime_error( this->__serverName + ": socket setsockopt, failed to make reusable socket" );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->__port);
	if (-1 == bind(this->__sd, (struct sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error( this->__serverName + ": bind syscall, failed to bind socket" );
	if (-1 == listen(this->__sd, 3))
		throw std::runtime_error( this->__serverName + ": listen syscall, failed to listen for connections" );
}

/***********************************************************************
 *                            STATIC METHODS                           *
 ***********************************************************************/

void		Server::setNonBlockingMode( int sd )
{
	/***************************************
	 * SET THE SOCKET TO NON BLOCKING MODE *
	 ***************************************/
	int flags = fcntl(sd, F_GETFL, 0);
	if (flags < 0 || fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error( "fcntl syscall, failed to make a non blocking socket" );
}
