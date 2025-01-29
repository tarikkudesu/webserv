#include "Headers.hpp"

Headers::Headers() : __contentLength(0),
					 __connectionType(KEEP_ALIVE),
					 __transferType(NONE)
{
}
Headers::Headers(const Headers &copy)
{
	*this = copy;
}
Headers &Headers::operator=(const Headers &assign)
{
	if (this != &assign)
	{
		this->__host = assign.__host;
		this->__port = assign.__port;
		this->__cookie = assign.__cookie;
		this->__contentType = assign.__contentType;
		this->__transferType = assign.__transferType;
		this->__contentLength = assign.__contentLength;
		this->__connectionType = assign.__connectionType;
		this->__transferEncoding = assign.__transferEncoding;
	}
	return *this;
}
Headers::~Headers()
{
}

void Headers::clear()
{
	this->__port = 8080;
	this->__host.clear();
	this->__cookie.clear();
	this->__boundary.clear();
	this->__contentLength = 0;
	this->__contentType.clear();
	this->__transferType = NONE;
	this->__transferEncoding.clear();
	this->__connectionType = KEEP_ALIVE;
}
String Headers::getHeaderFeildValue(const String &key, std::map<String, String> &headers)
{
	std::map<String, String>::iterator iter = headers.find(key);
	if (iter == headers.end())
		throw std::exception();
	return iter->second;
}
void Headers::contentLength(std::map<String, String> &headers)
{
	try
	{
		String value = getHeaderFeildValue("CONTENT-LENGTH", headers);
		this->__contentLength = wsu::stringToInt(value);
		if (this->__contentLength != 0)
			this->__transferType = DEFINED;
	}
	catch (std::exception &e)
	{
	}
}
void Headers::contentType(std::map<String, String> &headers)
{
	try
	{
		this->__contentType = getHeaderFeildValue("CONTENT-TYPE", headers);
		t_svec tmp = wsu::splitByChar(this->__contentType, ';');
		if (tmp.size() > 1)
		{
			if (tmp.at(0) == "multipart/form-data")
			{
				wsu::trimSpaces(tmp.at(1));
				t_svec t = wsu::splitByChar(tmp.at(1), '=');
				if (t.size() == 2 && t.at(0) == "boundary")
					this->__boundary = t.at(1);
				this->__transferType = MULTIPART;
			}
		}
	}
	catch (std::exception &e)
	{
	}
}

void Headers::connectionType(std::map<String, String> &headers)
{
	try
	{
		String value = getHeaderFeildValue("CONNECTION", headers);
		if (value == "close")
			this->__connectionType = CLOSE;
	}
	catch (std::exception &e)
	{
	}
}
void Headers::cookie(std::map<String, String> &headers)
{
	try
	{
		this->__cookie = getHeaderFeildValue("COOKIE", headers);
	}
	catch (std::exception &e)
	{
	}
}
void Headers::transferEncoding(std::map<String, String> &headers)
{
	try
	{
		this->__transferEncoding = getHeaderFeildValue("TRANSFER-ENCODING", headers);
		if (this->__transferEncoding == "chunked")
			this->__transferType = CHUNKED;
	}
	catch (std::exception &e)
	{
	}
}
void Headers::hostAndPort(std::map<String, String> &headers)
{
	try
	{
		String value = getHeaderFeildValue("HOST", headers);
		size_t pos = value.find(":");
		if (pos == String::npos)
		{
			this->__host = value;
			this->__port = 8080;
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
void Headers::parseHeaders(std::map<String, String> &headers)
{
	clear();
	hostAndPort(headers);
	connectionType(headers);
	contentLength(headers);
	transferEncoding(headers);
	contentType(headers);
	cookie(headers);
}

std::ostream &operator<<(std::ostream &os, const Headers &obj)
{
	os << "\t\tPort: " << obj.__port << std::endl;
	os << "\t\tHost: " << obj.__host << std::endl;
	os << "\t\tBoundary: " << obj.__boundary << std::endl;
	os << "\t\tContent-Type: " << obj.__contentType << std::endl;
	os << "\t\tContent-Length: " << obj.__contentLength << std::endl;
	os << "\t\tTransfer-Encoding: " << obj.__transferEncoding << "\n";
	os << "\t\tCookie: " << obj.__cookie << std::endl;
	return os;
}
