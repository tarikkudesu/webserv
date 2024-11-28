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
	*this = copy;
}

Connection::~Connection()
{

}

Connection	&Connection::operator=( const Connection &assign )
{
	if (this != &assign) {
		this->__sd = assign.__sd;
		this->__buff = assign.__buff;
		this->__erase = assign.__erase;
		this->__client = assign.__client;
		this->__requestBody = assign.__requestBody;
		this->__headerFeilds = assign.__headerFeilds;
		this->__responseQueue = assign.__responseQueue;
	}
	return *this;
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

bool	Connection::endConnection()
{
	return this->__client.connectionTypeClose();
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
	}
}
void		Connection::identifyRequestHeaders()
{
	String	currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t	pos = currBuff.find("\r\n\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestHeaders(currBuff.begin(), currBuff.begin() + pos + 2);
	this->__erase += pos + 4;
	this->__client.proccessHeaders(requestHeaders);
}
void		Connection::identifyRequestLine()
{
	String	currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t	pos = currBuff.find("\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestLine(currBuff.begin(), currBuff.begin() + pos);
	this->__erase += pos + 2;
	this->__client.proccessRequestLine(requestLine);
}
void	Connection::proccessInput( String input )
{
	if (input.empty() == false)
	{
		this->__buff += input;
		try
		{
			this->__erase = 0;
			this->__client.clear();
			identifyRequestLine();
			identifyRequestHeaders();
			identifyRequestBody();
			this->__buff.erase(0, this->__erase);
		} catch ( std::exception &e ) {
			this->__erase = 0;
		}
	}
}
