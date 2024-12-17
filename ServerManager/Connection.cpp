#include "Connection.hpp"

Connection::Connection() : __sd(-1),
						   __erase(0),
						   __serversP(NULL)
{
	WSU::error("Connection constructor");
}

Connection::Connection(int sd) : __sd(sd),
								 __erase(0),
								 __serversP(NULL)
{
}

Connection::Connection(const Connection &copy)
{
	WSU::error("Connection copy constructor");
	*this = copy;
}

Connection::~Connection()
{
}

Connection &Connection::operator=(const Connection &assign)
{
	WSU::error("Connection copy assignement operator");
	if (this != &assign)
	{
		__sd = assign.__sd;
		__serversP = assign.__serversP;
	}
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
		size_t contentLen = WSU::hexToInt(String(currBuff.begin(), currBuff.begin() + pos));
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
		if (this->__request.__transferEncoding == CHUNKED)
		{
			body = identifyChunks(currBuff);
		}
		else
		{
			size_t contentLen = this->__request.__contentLength;
			if (currBuff.length() <= contentLen)
				throw std::exception();
			body = String(currBuff.begin(), currBuff.begin() + contentLen);
			this->__erase += contentLen;
		}
		this->__request.__requestbody = body;
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
void Connection::requestParser()
{
	String requestLine = identifyRequestLine();
	String requestHeaders = identifyRequestHeaders();
	WSU::log(requestLine);
	this->__request.parseRequest(requestLine, requestHeaders);
	WSU::log("identify Body");
	identifyRequestBody();
	this->__buff.erase(0, this->__erase);
	this->__erase = 0;
	WSU::log("request proccessing complete");
}
Server *Connection::identifyServer()
{
	WSU::log("identifying server to respond");
	std::vector<Server *> tmpMapP;
	std::vector<Server *> tmpMapH;
	for (t_Server::iterator it = this->__serversP->begin(); it != this->__serversP->end(); it++)
	{
		if (it->second->getServerPort() == this->__request.__port)
			tmpMapP.push_back(it->second);
	}
	for (std::vector<Server *>::iterator it = tmpMapP.begin(); it != tmpMapP.end(); it++)
	{
		if ((*it)->amITheServerYouAreLookingFor(this->__request.__host) == false)
			tmpMapH.push_back(*it);
	}
	if (tmpMapH.empty())
		return this->__serversP->begin()->second;
	return tmpMapH.at(0);
}
void Connection::responseBuilder()
{
	Server *server = identifyServer();
	Response res(this->__request, *server);
}

void Connection::proccessData(String input)
{
	WSU::log("request proccessing");
	this->__buff += input;
	try
	{
		requestParser();
		responseBuilder();
		WSU::log("response building");
		const char *response = "HTTP/1.1 200 OK\n"
							   "Content-Type: text/html\n"
							   "Content-Length: 17\n"
							   "Connection: keep-alive\r\n\r\n"
							   "<h1>Webserv</h1>\n";
		this->__responseQueue.push(String(response));
		WSU::log("response proccessed");
	}
	catch (ErrorResponse &e)
	{
		WSU::log("response is an error page");
		this->__responseQueue.push(e.getResponse());
	}
	catch (std::exception &e)
	{
		WSU::log("continue");
		this->__erase = 0;
	}
}

// WSU::log(RED "+++++++++++++++++++ debug +++++++++++++++++++" RESET);
