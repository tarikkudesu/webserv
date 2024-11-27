#include "Connection.hpp"

Connection::Connection() :
__sd( -1 )
{

}

Connection::Connection( int sd ) :
__sd( sd )
{

}

Connection::Connection( const Connection &copy )
{
	(void) copy;
}

Connection::~Connection()
{
}

Connection	&Connection::operator=( const Connection &assign )
{
	(void) assign;
	return *this;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void	Connection::proccessInput( String input )
{
	std::cout << "proccessing\n";
	std::cout << input << "\n";
	std::cout << "done\n";
	exit(1); // temporarly exit
}
