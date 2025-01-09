#include "Headers.hpp"

Headers::Headers()
{
}
Headers::Headers(const Headers &copy)
{
}
Headers &Headers::operator=(const Headers &assign)
{
}
Headers::~Headers()
{
}

void Headers::clear()
{
	this->__contentLength = 0;
	this->__transferEncoding = GENERAL;
	this->__connectionType = KEEP_ALIVE;
}

String Headers::getHeaderFeildValue(const String &key, std::map<String, String> &headers)
{
	std::map<String, String>::iterator iter = headers.find(key);
	if (iter == headers.end())
		throw std::exception();
	return iter->second;
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
}
