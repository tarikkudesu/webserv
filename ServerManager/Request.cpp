#include "Request.hpp"

Request::Request() : __transferEncoding(GENERAL),
					 __connectionType(KEEP_ALIVE),
					 __contentLength(0)
{
}

Request::Request(const Request &copy)
{
	*this = copy;
}

Request::~Request()
{
}

Request &Request::operator=(const Request &assign)
{
	if (this != &assign)
	{
		this->__URI = assign.__URI;
		this->__method = assign.__method;
		this->__protocole = assign.__protocole;
		this->__headerFeilds = assign.__headerFeilds;
		this->__contentLength = assign.__contentLength;
		this->__connectionType = assign.__connectionType;
		this->__transferEncoding = assign.__transferEncoding;
	}
	return *this;
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/
int16_t Request::getPort() const
{
	return this->__port;
}
String Request::getHost() const
{
	return this->__host;
}
void	Request::setServer( Server &server )
{
	this->__server = &server;
}
void Request::clear()
{
	this->__URI.clear();
	this->__protocole.clear();
	this->__headerFeilds.clear();
	this->__transferEncoding = GENERAL;
}
bool Request::connectionTypeClose()
{
	if (this->__connectionType == CLOSE)
		return true;
	return false;
}
String Request::getHeaderFeildValue(const String &key)
{
	std::map<String, String>::iterator iter = __headerFeilds.find(key);
	if (iter == __headerFeilds.end())
		throw std::exception();
	return iter->second;
}
size_t Request::getContentLength()
{
	return this->__contentLength;
}
t_transferEncoding Request::gettransferEncoding()
{
	return this->__transferEncoding;
}
t_connectionType Request::getconnectionType()
{
	return this->__connectionType;
}
bool Request::hasBody()
{
	if (this->__transferEncoding == CHUNKED)
		return true;
	else if (this->__contentLength)
		return true;
	return false;
}
void Request::setBody(const String &body)
{
	this->__requestbody = body;
}
/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/
void Request::hostAndPort()
{
	try
	{
		String value = getHeaderFeildValue("Host");
		size_t pos = value.find(":");
		if (pos == String::npos)
		{
			this->__host = value;
			this->__port = 80;
		}
		else
		{
			this->__host = String(value.begin(), value.begin() + pos);
			String port = String(value.begin() + pos + 1, value.end());
			this->__port = std::strtol(port.c_str(), NULL, 10);
		}
	}
	catch (std::exception &e)
	{
		throw ErrorResponse(400, "No Host header feild");
	}
}
void Request::contentLength()
{
	try
	{
		std::istringstream ss(getHeaderFeildValue("Content-Length"));
		ss >> this->__contentLength;
	}
	catch (std::exception &e)
	{
	}
}
void Request::connectionType()
{
	try
	{
		String value = getHeaderFeildValue("connection");
		if (value == "close")
			this->__connectionType = CLOSE;
	}
	catch (std::exception &e)
	{
	}
}
void Request::transferEncoding()
{
	try
	{
		String value = getHeaderFeildValue("Transfer-Encoding");
		if (value.find("chunked") != String::npos)
			this->__transferEncoding = CHUNKED;
	}
	catch (std::exception &e)
	{
	}
}
void Request::proccessHeaders(String requestHeaders)
{
	size_t pos = 0;
	do
	{
		pos = requestHeaders.find("\r\n");
		if (pos == String::npos)
			break;
		{
			String hf(requestHeaders.begin(), requestHeaders.begin() + pos);
			size_t p = hf.find(": ");
			if (p == String::npos)
				throw ErrorResponse(400, "invalid Header feild");
			String key(hf.begin(), hf.begin() + p);
			String value(hf.begin() + p + 2, hf.end());
			std::cout << BLUE << key << RESET << ": " << YELLOW << value << RESET << "\n";
			if (key.empty() || String::npos != key.find_first_not_of(H_KEY_CHAR_SET))
				throw ErrorResponse(400, "invalid Header feild");
			this->__headerFeilds[key] = value;
			requestHeaders.erase(0, pos + 2);
		}

	} while (requestHeaders.empty() == false);
}
void Request::proccessRequestLine(const String &requestLine)
{
	if (2 != std::count(requestLine.begin(), requestLine.end(), ' '))
		throw ErrorResponse(400, "invalid Request Line (extra space)");
	std::istringstream iss(requestLine);
	String method, URI, protocole;
	iss >> method;
	iss >> URI;
	iss >> protocole;
	if (method.empty() || protocole.empty() || URI.empty())
		throw ErrorResponse(400, "invalid Request Line (METHOD URI PROTOCOLE)");
	if (method == "OPTIONS")
		this->__method = OPTIONS;
	else if (method == "GET")
		this->__method = GET;
	else if (method == "HEAD")
		this->__method = HEAD;
	else if (method == "POST")
		this->__method = POST;
	else if (method == "PUT")
		this->__method = PUT;
	else if (method == "DELETE")
		this->__method = DELETE;
	else if (method == "TRACE")
		this->__method = TRACE;
	else if (method == "CONNECT")
		this->__method = CONNECT;
	else
		throw ErrorResponse(501, "invalid method");
	this->__URI = URI;
	if (String::npos != URI.find_first_not_of(URI_CHAR_SET))
		throw ErrorResponse(400, "invalid URI (out of URI CHARSET)");
	this->__protocole = protocole;
	if (this->__protocole != PROTOCOLE_V)
		throw ErrorResponse(505, "Unsupported protocole");
}
void Request::parseRequest(const String &requestLine, const String &requestHeaders)
{
	clear();
	proccessRequestLine(requestLine);
	proccessHeaders(requestHeaders);
	transferEncoding();
	connectionType();
	contentLength();
	hostAndPort();
}