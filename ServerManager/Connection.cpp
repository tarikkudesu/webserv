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
	t_Server tmpMap;
	int id = 0;
	int16_t port = this->__request.getPort();
	String host = this->__request.getHost();
	for (t_Server::iterator it = this->__serversP->begin(); it != this->__serversP->end(); it++)
	{
		if (it->second->getServerPort() == port)
		{
			tmpMap[id] = it->second;
			id++;
		}
	}
	if (tmpMap.size() == 1)
		return *tmpMap.begin()->second;
	for (t_Server::iterator it = tmpMap.begin(); it != tmpMap.end(); it++)
	{

		// std::cout << it->second->getServerName() << "\n";
	}
	return *this->__serversP->begin()->second;
}
void Connection::requestParser()
{
	String requestLine = identifyRequestLine();
	WSU::l1();
	WSU::l1(requestLine);
	WSU::l1("\n");
	String requestHeaders = identifyRequestHeaders();
	this->__request.parseRequest(requestLine, requestHeaders);
	WSU::l1();
	WSU::l1("identify Body");
	WSU::l1("\n");
	identifyRequestBody();
	this->__buff.erase(0, this->__erase);
	WSU::l1();
	WSU::l1("request proccessing complete");
	WSU::l1("\n");
	WSU::l1();
	WSU::l1("identifying server to respond");
	WSU::l1("\n");
	this->__request.setServer(identifyServer());
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
		WSU::l1();
		WSU::l1("request proccessing");
		WSU::l1("\n");
		requestParser();
		responseBuilder();
		WSU::l1();
		WSU::l1("response building");
		WSU::l1("\n");
		const char *response = "HTTP/1.1 200 OK\n"
							   "Content-Type: text/html\n"
							   "Content-Length: 17\n"
							   "Connection: keep-alive\r\n\r\n"
							   "<h1>Webserv</h1>\n";
		this->__responseQueue.push(String(response));
		WSU::l1();
		WSU::l1("response proccessed");
		WSU::l1("\n");
	}
	catch (ErrorResponse &e)
	{
		WSU::l1();
		WSU::l1("response is an error page");
		WSU::l1("\n");
		this->__responseQueue.push(e.getResponse());
	}
	catch (std::exception &e)
	{
		WSU::terr(e.what());
		WSU::l1();
		WSU::l1("request not complete");
		WSU::l1("\n");
		this->__erase = 0;
	}
}
