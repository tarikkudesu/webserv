#include "Connection.hpp"

std::ofstream	Connection::__fs;

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
/***************************************************************************
 *                           RESPONSE PROCESSING                           *
 ***************************************************************************/
void Connection::processResponse()
{
	wsu::info("building the request");
	throw ErrorResponse(500, "still working on it, -- debuging --");
	Server *server = identifyServer();
	Location &location = server->identifyLocation(__request.__URI);
	Response res(this->__request, *server, location);
	this->__responseQueue.push(res.getResponse());
}
/***************************************************************************
 *                             BODY PROCESSING                             *
 ***************************************************************************/
void Connection::mpBoundry(t_multipartsection &part)
{
	size_t pos = __buff.find("\r\n");
	if (pos == String::npos)
		throw wsu::persist();
	String tmp(__buff.begin(), __buff.begin() + pos);
	__buff.erase(0, pos + 2);
	std::cout << tmp << "\n";
	if (tmp == "--" + __request.__headers.__boundry + "--")
	{
		Connection::__fs.close();
		__request.__phase = COMPLETE;
	}
	else if (tmp == "--" + __request.__headers.__boundry)
	{
		do
		{
			s_body	body;
			body._fileName = wsu::generateTimeBasedFileName();
			Connection::__fs.open(body._fileName.c_str());
			if (!Connection::__fs.good())
				continue ;
			__request.__body.push_back(body);
			part = MP_HEADERS;
			return ;
		} while (true);
	}
	else
		throw ErrorResponse(400, "multipart/form-data: boundry mismatch");
}
void Connection::mpHeaders(t_multipartsection &part)
{
	do
	{
		size_t pos = __buff.find("\r\n");
		if (pos == String::npos)
			throw wsu::persist();
		if (pos != 0)
		{
			String tmp(__buff.begin(), __buff.begin() + pos);
			__request.__body.back()._headers.push_back(tmp);
			__buff.erase(0, pos + 2);
		}
		else
		{
			__buff.erase(0, 2);
			break ;
		}
	} while (true);
	part = MP_BODY;
}
void Connection::mpBody(t_multipartsection &part)
{
	size_t pos = __buff.find("\r\n");
	if (pos == String::npos)
	{
		Connection::__fs << __buff;
		__buff.clear();
	}
	else
	{
		String tmp(__buff.begin(), __buff.begin() + pos);
		Connection::__fs << tmp;
		__buff.erase(0, pos + 2);
		part = MP_BOUNDRY;
	}
}
void Connection::processMultiPartBody()
{
	std::cout << "multipart body\n";
	static t_multipartsection part = MP_BOUNDRY;
	do
	{
		if (part == MP_BOUNDRY)
			mpBoundry(part);
		if (part == MP_HEADERS)
			mpHeaders(part);
		if (part == MP_BODY)
			mpBody(part);
	} while (true);
}
void Connection::processCunkedBody()
{
	std::cout << "chunked body\n";
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
	std::cout << "defined body\n";
	std::cout << __request.__headers.__contentLength << "\n";
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
	wsu::info("reading the body");
	if (__request.__headers.__transferType == DEFINED)
		processDefinedBody();
	else if (__request.__headers.__transferType == CHUNKED)
		processCunkedBody();
	else if (__request.__headers.__transferType == MULTIPART)
		processMultiPartBody();
}
void Connection::initializeTmpFiles()
{
	wsu::info("initializing temporary files");
	if (__request.__headers.__transferType == DEFINED \
		|| __request.__headers.__transferType == CHUNKED)
	{
		do
		{
			s_body	body;
			body._fileName = wsu::generateTimeBasedFileName();
			Connection::__fs.open(body._fileName.c_str());
			if (!Connection::__fs.good())
				continue ;
			__request.__body.push_back(body);
			break ;
		} while (true);
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
	wsu::info("proccessing request");
	String requestLine = identifyRequestLine();
	String requestHeaders = identifyRequestHeaders();
	this->__buff.erase(0, this->__erase);
	this->__erase = 0;
	this->__request.parseRequest(requestLine, requestHeaders);
	if (this->__request.__method == POST)
		__request.__phase = INITIALIZING;
	else
		__request.__phase = COMPLETE;
}
/**********************************************************************************
 *                                  PROCESS DATA                                  *
 **********************************************************************************/
void Connection::proccessData(String input)
{
	this->__buff += input;
	if (__buff.empty())
		return ;
	wsu::info("proccessing data");
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
	
	if (__request.__phase == NEWREQUEST)
	if (__request.__phase == INITIALIZING)
		std::cout << "INITIALIZING\n";
	if (__request.__phase == PROCESSING)
		std::cout << "PROCESSING\n";
	if (__request.__phase == COMPLETE)
		std::cout << "COMPLETE\n";
	wsu::info("completeeee--|" + __buff + "|");
}
