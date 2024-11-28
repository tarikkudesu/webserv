#include "Connection.hpp"

Connection::Connection() :
__sd( -1 )
{

}

Connection::Connection( int sd ) :
__sd( sd ),
__connectionType( KEEP_ALIVE ),
__erase( 0 )
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

void	Connection::identifyTransferEncoding()
{
	// try {
	// 	String value = this->__client.getHeaderFeildValue("Transfer-Encoding");
	// 	if (value == "close")
	// 		this->__connectionType = CLOSE;
	// } catch ( std::exception &e ) {
	// 	this->__connectionType = CLOSE;
	// }
}
void	Connection::identifyConnectionType()
{
	try {
		String value = this->__client.getHeaderFeildValue("connection");
		if (value == "close")
			this->__connectionType = CLOSE;
	} catch ( std::exception &e ) {
		this->__connectionType = CLOSE;
	}
}
String		Connection::identifyHeaders()
{
	size_t	pos = this->__buff.find("\r\n\r\n", this->__erase);
	if (pos == String::npos)
		throw std::exception();
	String requestLine(this->__buff.begin() + this->__erase, this->__buff.begin() + this->__erase + pos);
	this->__erase += pos + 4;
	// this->__buff.erase(0, pos + 4);
	return requestLine;
}
String		Connection::identifyRequestLine()
{
	size_t	pos = this->__buff.find("\r\n", this->__erase);
	if (pos == String::npos)
		throw std::exception();
	String requestLine(this->__buff.begin() + this->__erase, this->__buff.begin() + this->__erase + pos);
	this->__erase += pos + 2;
	// this->__buff.erase(0, pos + 2);
	return requestLine;
}
void	Connection::proccessInput( String input )
{

	if (input.empty() == false)
	{
		this->__buff += input;
		try
		{
			String requestLine = identifyRequestLine();
			String requestHeaders = identifyHeaders();
			std::cout << requestLine << "\n\n" << requestHeaders << "\n\n";
			{
				Client	client(requestLine, requestHeaders);
				this->__client = client;
			}
			this->__client.parseRequest();
			identifyConnectionType();
		} catch ( std::exception &e ) {
			this->__erase = 0;
		}
	}
	// exit(1); // temporarly exit
}
