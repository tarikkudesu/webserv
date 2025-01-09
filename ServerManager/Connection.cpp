#include "Connection.hpp"

Connection::Connection() : __sd(-1),
						   __erase(0),
						   __serversP(NULL),
						   __readable(true)
{
	wsu::debug("Connection default constructor");
}
Connection::Connection(int sd) : __sd(sd),
								 __erase(0),
								 __serversP(NULL),
								 __readable(false)
{
	wsu::debug("Server single para constructor");
}
Connection::Connection(const Connection &copy)
{
	wsu::debug("Connection copy constructor");
	*this = copy;
}
Connection &Connection::operator=(const Connection &assign)
{
	wsu::debug("Connection copy assignement operator");
	if (this != &assign)
	{
		__sd = assign.__sd;
		__serversP = assign.__serversP;
		__readable = assign.__readable;
		__responseQueue = assign.__responseQueue;
	}
	return *this;
}
Connection::~Connection()
{
	wsu::debug("Connection destructor");
}

void Connection::setServers(t_Server &servers)
{
	this->__serversP = &servers;
}
int Connection::getSock()
{
	return __sd;
}
/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

String Connection::identifyChunks(String &currBuff)
{
	(void)currBuff；
	// String body;
	// do
	// {
	// 	size_t pos = currBuff.find("\r\n");
	// 	if (pos == String::npos)
	// 		throw std::exception();
	// 	size_t contentLen = wsu::hexToInt(String(currBuff.begin(), currBuff.begin() + pos));
	// 	currBuff.erase(0, pos + 2);
	// 	this->__erase += pos + 2;
	// 	if (contentLen == 0)
	// 	{
	// 		this->__erase += 2;
	// 		break;
	// 	}
	// 	if (this->__buff.length() < this->__erase + contentLen + 2)
	// 		throw std::exception();
	// 	pos = currBuff.find("\r\n");
	// 	if (pos == String::npos)
	// 		throw std::exception();
	// 	if (pos != contentLen)
	// 		throw ErrorResponse(400, "chunk size mismatch");
	// 	String chunk(String(currBuff.begin(), currBuff.begin() + contentLen));
	// 	currBuff.erase(0, contentLen + 2);
	// 	this->__erase += contentLen + 2;
	// 	body += chunk;
	// } while (true);
	// return body;
}
void Connection::identifyRequestBody()
{
	// if (this->__request.hasBody())
	// {
	// 	String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	// 	String body;
	// 	if (this->__request.__headers.__transferEncoding == CHUNKED)
	// 	{
	// 		body = identifyChunks(currBuff);
	// 	}
	// 	else
	// 	{
	// 		size_t contentLen = this->__request.__headers.__contentLength;
	// 		if (contentLen >= READ_SIZE)
	// 			this->__readable = true;
	// 		if (currBuff.length() < contentLen)
	// 			throw std::exception();
	// 		body = String(currBuff.begin(), currBuff.begin() + contentLen);
	// 		this->__erase += contentLen;
	// 	}
	// 	this->__request.__requestbody = body;
	// }
}
Server *Connection::identifyServer()
{
	wsu::info("identifying server");
	t_serVect tmpMapP, tmpMapH;
	for (t_Server::iterator it = this->__serversP->begin(); it != this->__serversP->end(); it++)
	{
		if (it->second->getServerPort() == this->__request.__headers.__port)
			tmpMapP.push_back(it->second);
	}
	for (t_serVect::iterator it = tmpMapP.begin(); it != tmpMapP.end(); it++)
	{
		if ((*it)->amITheServerYouAreLookingFor(this->__request.__headers.__host) == false)
			tmpMapH.push_back(*it);
	}
	if (tmpMapH.empty())
		return this->__serversP->begin()->second;
	return tmpMapH.at(0);
}
void Connection::processResponse()
{
	if (wsu::__criticalOverLoad == true)
		throw ErrorResponse(503, "critical server overload");
	Server *server = identifyServer();
	Location &location = server->identifyLocation(__request.__URI);
	Response res(this->__request, *server, location);
	this->__responseQueue.push(res.getResponse());
}
String Connection::identifyRequestHeaders()
{
	String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t pos = currBuff.find("\r\n\r\n");
	if (pos == String::npos)
		throw wsu::persist();
	String requestHeaders(currBuff.begin(), currBuff.begin() + pos + 2);
	this->__erase += pos + 4;
	return requestHeaders;
}
String Connection::identifyRequestLine()
{
	String currBuff(this->__buff.begin() + this->__erase, this->__buff.end());
	size_t pos = currBuff.find("\r\n");
	if (pos == String::npos)
		throw wsu::persist();
	String requestLine(currBuff.begin(), currBuff.begin() + pos);
	this->__erase += pos + 2;
	if (requestLine.length() >= 4094)
		throw ErrorResponse(400, "Oversized request line ( 4094Bytes )");
	return requestLine;
}
void Connection::processRequest()
{
	if (__request.__phase == COMPLETE)
	{
		this->__response.clear();
		String requestLine = identifyRequestLine();
		String requestHeaders = identifyRequestHeaders();
		std::cout << requestLine << "\n" << requestHeaders << "\n";
		this->__buff.erase(0, this->__erase);
		this->__erase = 0;
		this->__request.parseRequest(requestLine, requestHeaders);
	}
	else
	{
		__request.__phase = PROCESSING;
		// complete reading
		// phase should be set on a condition

	}
}
void Connection::proccessData(String input)
{
	wsu::info("request proccessing");
	this->__buff += input;
	try
	{
		processRequest();
		processResponse();
	}
	catch (ErrorResponse &e)
	{
		this->__responseQueue.push(e.getResponse());
	}
	catch (wsu::persist &e)
	{
		wsu::error(e.what());
		this->__erase = 0;
	}
	catch (std::exception &e)
	{
		wsu::error(e.what());
		this->__erase = 0;
	}
}
