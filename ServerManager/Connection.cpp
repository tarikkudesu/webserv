#include "Connection.hpp"

Connection::Connection() :
__sd( -1 )
{
	(void)__sd;
}

Connection::Connection( int sd ) :
__sd( sd ),
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

void	Connection::identifyRequestBody()
{
	if (this->__client.hasBody())
	{
		String	currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
		String	body;
		if (this->__client.gettransferEncoding() == CHUNKED) {
			do {
				size_t	pos = currBuff.find("\r\n");
				if (pos == String::npos)
					throw std::exception();
				size_t contentLen = 0;
				{
					std::stringstream	ss;
					String	hex(currBuff.begin(), currBuff.begin() + pos);
					ss << std::hex << hex;
					ss >> contentLen;
				}
				currBuff.erase(0, pos + 2);
				this->__erase += pos + 2;
				if (contentLen == 0) {
					this->__erase += 2;
					break ;
				}
				if (this->__buff.length() < this->__erase + contentLen + 2)
					throw std::exception();
				String	chunk(String(currBuff.begin(), currBuff.begin() + contentLen));
				currBuff.erase(0, contentLen + 2);
				this->__erase += contentLen + 2;
				body += chunk;
			} while (true);
		} else {
			size_t	contentLen = this->__client.getContentLength();
			if (currBuff.length() <= contentLen)
				throw std::exception();
			body = String(currBuff.begin(), currBuff.begin() + contentLen);
			this->__erase += contentLen;
		}
		this->__client.setBody(body);
		std::cout << MAGENTA << body << RESET << "\n";
	}
}
String		Connection::identifyRequestHeaders()
{
	String	currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t	pos = currBuff.find("\r\n\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestHeaders(currBuff.begin(), currBuff.begin() + pos + 2);
	this->__erase += pos + 4;
	return requestHeaders;
}
String		Connection::identifyRequestLine()
{
	String	currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t	pos = currBuff.find("\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestLine(currBuff.begin(), currBuff.begin() + pos);
	this->__erase += pos + 2;
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
			String requestHeaders = identifyRequestHeaders();
			std::cout << YELLOW << requestLine << RESET << "\n";
			this->__client.parseRequest(requestLine, requestHeaders);
			identifyRequestBody();
			this->__buff.erase(0, this->__erase);
		} catch ( std::exception &e ) {
			this->__erase = 0;
		}
	}
	// exit(1); // temporarly exit
}
