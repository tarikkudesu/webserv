#include "Connection.hpp"

std::ofstream Connection::__fs;

Connection::Connection() : __sd(-1),
						   __erase(0),
						   __serversP(NULL)
{
	wsu::debug("Connection default constructor");
}
Connection::Connection(int sd) : __sd(sd),
								 __erase(0),
								 __serversP(NULL)
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
Server *Connection::identifyServer()
{
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
/***************************************************************************
 *                           RESPONSE PROCESSING                           *
 ***************************************************************************/
void Connection::processResponse()
{
	Server *server = identifyServer();
	Location &location = server->identifyLocation(__request.__URI);
	Response res(this->__request, *server, location);
	this->__responseQueue.push(res.getResponse());
	__request.__phase = NEWREQUEST;
}
/***************************************************************************
 *                             BODY PROCESSING                             *
 ***************************************************************************/
void Connection::mpHeaders(t_multipartsection &part)
{
	size_t pos = __buff.find("\r\n\r\n");
	if (pos == String::npos)
		throw wsu::persist();
	do
	{
		s_body body;
		body._fileName = wsu::generateTimeBasedFileName();
		Connection::__fs.open(body._fileName.c_str());
		if (!Connection::__fs.good())
		{
			wsu::warn("could no create temporary file: " + body._fileName);
			continue ;
		}
		__request.__body.push_back(body);
		part = MP_BODY;
		break;
	} while (true);
	do
	{
		size_t p = __buff.find("\r\n");
		if (p == 0 || p == String::npos)
			break;
		String header(__buff.begin(), __buff.begin() + p);
		__request.__body.back()._headers.push_back(header);
		__buff.erase(0, p + 2);
	} while (true);
	__buff.erase(0, 2);
}
void Connection::mpBody(t_multipartsection &part)
{
	size_t end = __buff.find("\r\n--" + __request.__headers.__boundary + "--\r\n");
	size_t pos = __buff.find("\r\n--" + __request.__headers.__boundary + "\r\n");
	if (pos == String::npos && end == String::npos)
	{
		size_t len = __request.__headers.__boundary.length() + 8;
		if (__buff.length() <= len)
			throw wsu::persist();
		String data(__buff.begin(), __buff.end() - len);
		Connection::__fs << data;
		__buff.erase(0, data.length());
	}
	else if (pos != String::npos)
	{
		size_t len = __request.__headers.__boundary.length() + 6;
		String data(__buff.begin(), __buff.begin() + pos);
		Connection::__fs << data;
		Connection::__fs.close();
		__buff.erase(0, pos + len);
		part = MP_HEADERS;
	}
	else if (end != String::npos)
	{
		size_t len = __request.__headers.__boundary.length() + 8;
		String data(__buff.begin(), __buff.begin() + end);
		Connection::__fs << data;
		Connection::__fs.close();
		__buff.erase(0, end + len);
		__request.__phase = COMPLETE;
		part = MP_HEADERS;
	}
}
void Connection::processMultiPartBody()
{
	static t_multipartsection part = MP_HEADERS;
	do
	{
		if (part == MP_HEADERS)
			mpHeaders(part);
		if (part == MP_BODY)
			mpBody(part);
	} while(__request.__phase != COMPLETE);
}
void Connection::processCunkedBody()
{
	static size_t chunkSize;
	do
	{
		if (chunkSize == 0)
		{
			size_t pos = __buff.find("\r\n");
			if (pos == String::npos)
				throw wsu::persist();
			String hex = String(__buff.begin(), __buff.begin() + pos);
			chunkSize = wsu::hexToInt(hex);
			__buff.erase(0, pos + 2);
			if (chunkSize == 0)
			{
				Connection::__fs.close();
				__request.__phase = COMPLETE;
				return;
			}
		}
		if (chunkSize < __buff.length())
		{
			String tmp = String(__buff.begin(), __buff.begin() + chunkSize);
			Connection::__fs << tmp;
			__buff.erase(0, chunkSize);
			chunkSize -= tmp.length();
		}
		else
		{
			Connection::__fs << __buff;
			chunkSize -= __buff.length();
			__buff.clear();
			break;
		}
	} while (true);
}
void Connection::processDefinedBody()
{
	if (__request.__headers.__contentLength < __buff.length())
	{
		String tmp = String(__buff.begin(), __buff.begin() + __request.__headers.__contentLength);
		__request.__headers.__contentLength -= tmp.length();
		__buff = __buff.substr(__request.__headers.__contentLength);
		Connection::__fs << tmp;
	}
	else
	{
		Connection::__fs << __buff;
		__request.__headers.__contentLength -= __buff.length();
		__buff.clear();
	}
	if (__request.__headers.__contentLength == 0)
	{
		__request.__phase = COMPLETE;
		Connection::__fs.close();
	}
}
void Connection::indentifyRequestBody()
{
	if (__request.__headers.__transferType == DEFINED)
		processDefinedBody();
	else if (__request.__headers.__transferType == CHUNKED)
		processCunkedBody();
	else if (__request.__headers.__transferType == MULTIPART)
		processMultiPartBody();
}
void Connection::initializeTmpFiles()
{
	if (__request.__headers.__transferType == DEFINED || __request.__headers.__transferType == CHUNKED)
	{
		do
		{
			s_body body;
			body._fileName = wsu::generateTimeBasedFileName();
			Connection::__fs.open(body._fileName.c_str());
			if (!Connection::__fs.good())
				continue;
			__request.__body.push_back(body);
			break;
		} while (true);
	}
	else if (__request.__headers.__transferType == MULTIPART)
	{
		size_t pos = __buff.find("--" + __request.__headers.__boundary + "\r\n");
		if (pos == String::npos)
			throw wsu::persist();
		else if (pos != 0)
			throw ErrorResponse(400, "Multipart/data-from: boundry mismatch");
		__buff.erase(0, __request.__headers.__boundary.length() + 4);
	}
	__request.__phase = PROCESSING;
}
/******************************************************************************
 *                             REQUEST PROCESSING                             *
 ******************************************************************************/
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
	String requestLine = identifyRequestLine();
	String requestHeaders = identifyRequestHeaders();
	this->__buff.erase(0, this->__erase);
	this->__erase = 0;
	this->__request.parseRequest(requestLine, requestHeaders);
	if (__request.__method == POST)
		__request.__phase = INITIALIZING;
	else
		__request.__phase = COMPLETE;
}
/**********************************************************************************
 *                                  PROCESS DATA                                  *
 **********************************************************************************/
void Connection::proccessData(char *input, ssize_t bytesRead)
{
	this->__data.join(cString(input, bytesRead));
	this->__buff += input;
	try
	{
		if (__request.__phase == NEWREQUEST)
			processRequest();
		if (__request.__phase == INITIALIZING)
			initializeTmpFiles();
		if (__request.__phase == PROCESSING)
			indentifyRequestBody();
		if (__request.__phase == COMPLETE)
			processResponse();
	}
	catch (ErrorResponse &e)
	{
		this->__responseQueue.push(e.getResponse());
		__request.__phase = NEWREQUEST;
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
