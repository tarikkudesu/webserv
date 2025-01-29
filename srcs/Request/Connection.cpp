#include "Connection.hpp"

std::ofstream Connection::__fs;

Connection::Connection() : __sd(-1),
                           __serversP(NULL)
{
    wsu::debug("Connection default constructor");
}
Connection::Connection(int sd) : __sd(sd),
                                 __serversP(NULL)
{
    wsu::debug("Connection single para constructor");
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
    wsu::info("COMPLETE");
    Server *server = identifyServer();
    Location &location = server->identifyLocation(__request.__URI);
    Response res(this->__request, *server, location);
    const std::vector< BasicString >    &response = res.getResponse();
    for (std::vector<BasicString>::const_iterator it = response.begin(); it != response.end(); it++)
        this->__responseQueue.push(*it);
    __request.__phase = NEWREQUEST;
}
/***************************************************************************
 *                             BODY PROCESSING                             *
 ***************************************************************************/
void Connection::processCunkedBody()
{
    static size_t chunkSize;
    do
    {
        if (chunkSize == 0)
        {
            size_t pos = __data.find("\r\n");
            if (pos == String::npos)
                throw wsu::persist();
            BasicString hex = __data.substr(0, pos);
            chunkSize = wsu::hexToInt(hex.to_string());
            __data.erase(0, pos + 2);
            if (chunkSize == 0)
            {
                Connection::__fs.close();
                __request.__phase = COMPLETE;
                return;
            }
        }
        if (chunkSize < __data.length())
        {
            BasicString tmp = __data.substr(0, chunkSize);
            Connection::__fs << tmp;
            __data.erase(0, chunkSize);
            chunkSize -= tmp.length();
        }
        else
        {
            Connection::__fs << __data;
            chunkSize -= __data.length();
            __data.clear();
            break;
        }
    } while (true);
}
void Connection::processDefinedBody()
{
    if (__request.__headers.__contentLength < __data.length())
    {
        BasicString tmp = __data.substr(0, __request.__headers.__contentLength);
        __request.__headers.__contentLength -= tmp.length();
        __data.erase(0, tmp.length());
        Connection::__fs << tmp;
    }
    else
    {
        Connection::__fs << __data;
        __request.__headers.__contentLength -= __data.length();
        __data.clear();
    }
    if (__request.__headers.__contentLength == 0)
    {
        __request.__phase = COMPLETE;
        Connection::__fs.close();
    }
}
void Connection::mpHeaders(t_multipartsection &part)
{
    size_t pos = __data.find("\r\n\r\n");
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
        size_t p = __data.find("\r\n");
        if (p == 0 || p == String::npos)
            break;
        BasicString header = __data.substr(0, p);
        __request.__body.back()._headers.push_back(header.to_string());
        __data.erase(0, p + 2);
    } while (true);
    __data.erase(0, 2);
}
void Connection::mpBody(t_multipartsection &part)
{
    size_t end = __data.find("\r\n--" + __request.__headers.__boundary + "--\r\n");
    size_t pos = __data.find("\r\n--" + __request.__headers.__boundary + "\r\n");
    if (pos == String::npos && end == String::npos)
    {
        size_t len = __request.__headers.__boundary.length() + 8;
        if (__data.length() <= len)
            throw wsu::persist();
        BasicString data = __data.substr(0, __data.length() - len);
        Connection::__fs << data;
        __data.erase(0, data.length());
    }
    else if (pos != String::npos)
    {
        size_t len = __request.__headers.__boundary.length() + 6;
        BasicString data = __data.substr(0, pos);
        Connection::__fs << data;
        Connection::__fs.close();
        __data.erase(0, pos + len);
        part = MP_HEADERS;
    }
    else if (end != String::npos)
    {
        size_t len = __request.__headers.__boundary.length() + 8;
        BasicString data = __data.substr(0, end);
        Connection::__fs << data;
        Connection::__fs.close();
        __data.erase(0, end + len);
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
void Connection::indentifyRequestBody()
{
    wsu::info("PROCESSING");
    if (__request.__headers.__transferType == DEFINED)
        processDefinedBody();
    else if (__request.__headers.__transferType == CHUNKED)
        processCunkedBody();
    else if (__request.__headers.__transferType == MULTIPART)
        processMultiPartBody();
}
void Connection::initializeTmpFiles()
{
    wsu::info("INITIALIZING");
    if (__request.__headers.__transferType == DEFINED || __request.__headers.__transferType == CHUNKED)
    {
        wsu::warn("defined || chunked");
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
        size_t pos1 = __data.find("--" + __request.__headers.__boundary + "\r\n");
        size_t pos2 = __data.find("--" + __request.__headers.__boundary + "--\r\n");
        if (pos1 == String::npos && pos2 == String::npos)
            throw wsu::persist();
        else
        {
            if (pos2 == 0)
            {
                __data.erase(0, __request.__headers.__boundary.length() + 6);
                throw ErrorResponse(400, "No files were uploaded");
            }
            if (pos1 == String::npos)
                throw wsu::persist();
            else if (pos1 != 0)
            {
                __data.erase(0, __request.__headers.__boundary.length() + 6);
                throw ErrorResponse(400, "Multipart/data-from: boundry mismatch");
            }
        }
        __data.erase(0, __request.__headers.__boundary.length() + 4);
    }
    __request.__phase = PROCESSING;
}
/******************************************************************************
 *                             REQUEST PROCESSING                             *
 ******************************************************************************/

void Connection::processRequest()
{
    wsu::info("NEWREQUEST");
    size_t s = __data.find("\r\n");
    size_t h = __data.find("\r\n\r\n");
    if (s == String::npos || h == String::npos)
        throw wsu::persist();
    h -= (s + 2);
    BasicString	requestLine = __data.substr(0, s);
    __data.erase(0, s + 2);
    BasicString	requestHeaders = __data.substr(0, h + 2);
    __data.erase(0, h + 4);
    if (requestLine.length() >= 4094)
        throw ErrorResponse(400, "Oversized request line ( 4094Bytes )");
    this->__request.parseRequest(requestLine.to_string(), requestHeaders.to_string());
    if (__request.__method == POST)
        __request.__phase = INITIALIZING;
    else
        __request.__phase = COMPLETE;
}
/**********************************************************************************
 *                                  PROCESS DATA                                  *
 **********************************************************************************/
void Connection::proccessData(BasicString input)
{
    this->__data.join(input);
    try
    {
        wsu::info("proccessing request");
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
    catch (Request &e)
    {
        processResponse();
    }
    catch (wsu::persist &e)
    {
        wsu::error("presist");
    }
    catch (std::exception &e)
    {
    }
}
