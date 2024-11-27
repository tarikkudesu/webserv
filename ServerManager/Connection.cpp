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

String		Connection::identifyRequestLine( String::iterator &start, String::iterator &end )
{
	String	input(start, end);
	size_t	pos = input.find("\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestLine(start, start + pos);
	start += pos + 2;
	return requestLine;
}
String		Connection::identifyHeaders( String::iterator &start, String::iterator &end )
{
	String	input(start, end);
	size_t	pos = input.find("\r\n\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestHeaders(start, start + pos);
	start += pos + 4;
	return requestHeaders;
}
void	Connection::proccessInput( String input )
{

	if (input.empty() == false)
	{
		try
		{
			String::iterator start = input.begin();
			String::iterator end = input.end();
			String requestLine = identifyRequestLine(start, end);
			String requestHeaders = identifyHeaders(start, end);
			Client	client(requestLine, requestHeaders);
		} catch ( std::exception &e ) {
			return ;
		}
	}
	exit(1); // temporarly exit
}
