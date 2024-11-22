#include "Server.hpp"

Server::Server( )
{

}

Server::Server( const Server &copy ) :
__sd( copy.__sd ),
__port( copy.__port ),
__serverName( copy.__serverName )
{

}

Server::~Server() {}

Server	&Server::operator=( const Server &assign )
{
	if (this != &assign) {
		__sd = assign.__sd;
		__port = assign.__port;
		__serverName = assign.__serverName;
	}	
	return *this;
}

void		Server::setNonBlockingMode( int sd )
{
	/***************************************
	 * SET THE SOCKET TO NON BLOCKING MODE *
	 ***************************************/
	int flags = fcntl(sd, F_GETFL, 0);
	if (flags < 0 || fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error( "fcntl syscall, failed to make a non blocking socket" );
	Logs::tout( "fcntl" );
}

void	Server::setup()
{
	struct sockaddr_in	addr;
	int					reuse = 1;

	std::cout << "\n" << this->__serverName << "\n";
	this->__sd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->__sd == -1)
		throw std::runtime_error( this->__serverName + ": socket syscall, failed to create socket" );
	Logs::tout( "socket" );
	if (-1 == setsockopt(__sd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)))
		throw std::runtime_error( this->__serverName + ": socket setsockopt, failed to make reusable socket" );
	Logs::tout( "setsockopt" );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->__port);
	if (-1 == bind(this->__sd, (struct sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error( this->__serverName + ": bind syscall, failed to bind socket" );
	Logs::tout( "bind" );
	if (-1 == listen(this->__sd, 3))
		throw std::runtime_error( this->__serverName + ": listen syscall, failed to listen for connections" );
	Logs::tout( "listen" );
	Server::setNonBlockingMode( this->__sd );
}
