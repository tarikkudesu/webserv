#include "Connection.hpp"

Connection::Connection() : __sd(-1),
						   __erase(0),
						   __serversP(NULL)
{
	(void)__sd;
	// std::cout << __sd << " default constructor\n";
}

Connection::Connection(int sd) : __sd(sd),
								 __erase(0),
								 __serversP(NULL)
{
	// std::cout << __sd << " parameterized constructor\n";
}

Connection::Connection(const Connection &copy)
{
	(void)copy;
}

Connection::~Connection()
{
	// std::cout << __sd << " destructor called\n";
}

Connection &Connection::operator=(const Connection &assign)
{
	(void)assign;
	return *this;
}

void Connection::setServers(t_Server &servers)
{
	this->__serversP = &servers;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

String Connection::identifyChunks(String &currBuff)
{
	String body;
	do
	{
		size_t pos = currBuff.find("\r\n");
		if (pos == String::npos)
			throw std::exception();
		size_t contentLen = 0;
		{
			std::stringstream ss;
			String hex(currBuff.begin(), currBuff.begin() + pos);
			ss << std::hex << hex;
			ss >> contentLen;
		}
		currBuff.erase(0, pos + 2);
		this->__erase += pos + 2;
		if (contentLen == 0)
		{
			this->__erase += 2;
			break;
		}
		if (this->__buff.length() < this->__erase + contentLen + 2)
			throw std::exception();
		pos = currBuff.find("\r\n");
		if (pos == String::npos)
			throw std::exception();
		if (pos != contentLen)
			throw ErrorResponse(400, "chunk size mismatch");
		String chunk(String(currBuff.begin(), currBuff.begin() + contentLen));
		currBuff.erase(0, contentLen + 2);
		this->__erase += contentLen + 2;
		body += chunk;
	} while (true);
	return body;
}
void Connection::identifyRequestBody()
{
	if (this->__request.hasBody())
	{
		String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
		String body;
		if (this->__request.gettransferEncoding() == CHUNKED)
		{
			body = identifyChunks(currBuff);
		}
		else
		{
			size_t contentLen = this->__request.getContentLength();
			if (currBuff.length() <= contentLen)
				throw std::exception();
			body = String(currBuff.begin(), currBuff.begin() + contentLen);
			this->__erase += contentLen;
		}
		this->__request.setBody(body);
		std::cout << MAGENTA << body << RESET << "\n";
	}
}
String Connection::identifyRequestHeaders()
{
	String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t pos = currBuff.find("\r\n\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestHeaders(currBuff.begin(), currBuff.begin() + pos + 2);
	this->__erase += pos + 4;
	return requestHeaders;
}
String Connection::identifyRequestLine()
{
	String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t pos = currBuff.find("\r\n");
	if (pos == String::npos)
		throw std::exception();
	String requestLine(currBuff.begin(), currBuff.begin() + pos);
	this->__erase += pos + 2;
	if (requestLine.length() >= 4094)
		throw ErrorResponse(400, "Oversized request line ( 4094Bytes )");
	return requestLine;
}
Server &Connection::identifyServer()
{
	std::cout << "identifying server ...\n";
	int16_t	port = this->__request.getPort();
}
void Connection::requestParser()
{
	String requestLine = identifyRequestLine();
	String requestHeaders = identifyRequestHeaders();
	this->__request.parseRequest(requestLine, requestHeaders);
	identifyRequestBody();
	this->__buff.erase(0, this->__erase);
	this->__request.setServer( identifyServer() );
	this->__erase = 0;
}
void Connection::responseBuilder()
{
}

void Connection::proccessData(String input)
{
	this->__buff += input;
	try
	{
		requestParser();
		responseBuilder();
		const char *response = "HTTP/1.1 200 OK\n"
							   "Content-Type: text/html\n"
							   "Content-Length: 17\n"
							   "Connection: keep-alive\r\n\r\n"
							   "<h1>Webserv</h1>\n";
		this->__responseQueue.push(String(response));
		Logs::tout("read");
	}
	catch (ErrorResponse &e)
	{
		this->__responseQueue.push(e.getRespose());
	}
	catch (std::exception &e)
	{
		this->__erase = 0;
	}
}
