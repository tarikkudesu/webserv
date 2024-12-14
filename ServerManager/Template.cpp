#include "Template.hpp"

Template::Template()
{
}
Template::Template(String &line) : __line(line),
								   __host("0.0.0.0"),
								   __clientBodyBufferSize(8000),
								   b__clientBodyBufferSize(false),
								   b__host(false)
{
	__ports.push_back(8080);
	parse();
}

Template::Template(const Template &copy)
{
	*this = copy;
}

Template::~Template()
{
}

Template &Template::operator=(const Template &assign)
{
	(void)assign;
	return *this;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void Template::proccessHostToken(std::vector<String> &tokens)
{
	this->__host.clear();
	if (this->b__host)
		throw std::runtime_error(tokens.at(0) + ": multiple directives");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no host value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple host values");
	this->__host = tokens.at(1);
	this->b__host = true;
}
void Template::proccessListenToken(std::vector<String> &tokens)
{
	this->__ports.clear();
	if (tokens.size() > 80) // an extra leyer of protection, this value can be changed later
	{
		throw std::runtime_error(tokens.at(0) + ": this amount of ports is excessive");
	}
	else if (tokens.size() > 1)
	{
		for (std::vector<String>::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (String::npos != it->find_first_not_of("0123456789"))
				throw std::runtime_error(tokens.at(0) + ": invalid port: not a number");
			size_t port = WSU::stringToInt(*it);
			if (port > 65535)
				throw std::runtime_error(tokens.at(0) + ": invalid port: out of range");
			this->__ports.push_back(port);
		}
	}
	else
	{
		throw std::runtime_error(tokens.at(0) + ": no port value");
	}
}
void Template::proccessServerNameToken(std::vector<String> &tokens)
{
	if (tokens.size() > 20) // an extra leyer of protection, this value can be changed later
	{
		throw std::runtime_error(tokens.at(0) + ": this amount of server names is excessive");
	}
	else if (tokens.size() > 1)
	{
		for (std::vector<String>::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (String::npos != it->find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_*"))
				throw std::runtime_error(tokens.at(0) + ": invalid server_name: out of character range");
			this->__serverNames.push_back(*it);
		}
	}
	else
	{
		throw std::runtime_error(tokens.at(0) + ": no server_name value");
	}
}
void Template::proccessClientBodyBufferSizeToken(std::vector<String> &tokens)
{
	if (this->b__clientBodyBufferSize)
		throw std::runtime_error(tokens.at(0) + ": multiple directives");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no client_body_buffer_size value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple client_body_buffer_size values");
	if (String::npos != tokens.at(1).find_first_not_of("0123456789"))
		throw std::runtime_error(tokens.at(0) + ": invalid client_body_buffer_size: not a number");
	this->__clientBodyBufferSize = WSU::stringToInt(tokens.at(1));
	this->b__clientBodyBufferSize = true;
}

void Template::proccessToken(std::vector<String> &tokens)
{
	String &key = tokens.at(0);
	if (key != "host" &&
		key != "root" &&
		key != "index" &&
		key != "listen" &&
		key != "autoindex" &&
		key != "error_page" &&
		key != "server_name" &&
		key != "allow_methods" &&
		key != "client_body_buffer_size")
		throw std::runtime_error(key + ": unknown directive");
	if (key == "host")
		proccessHostToken(tokens);
	else if (key == "listen")
		proccessListenToken(tokens);
	else if (key == "server_name")
		proccessServerNameToken(tokens);
	else if (key == "client_body_buffer_size")
		proccessClientBodyBufferSizeToken(tokens);
}
void Template::proccessDirectives()
{
	for (std::deque<String>::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		std::vector<String> tokens = WSU::splitBySpaces(*it);
		if (!tokens.empty())
		{
			proccessToken(tokens);
		}
	}
}
void Template::LocationBlock(size_t pos)
{
	size_t end = pos + 1;
	size_t tracker = 1;

	do
	{
		if (end >= this->__line.length())
			break;
		if (this->__line.at(end) == '}')
			tracker--;
		if (this->__line.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	String locationBlock(this->__line.begin(), this->__line.begin() + end);
	WSU::trimSpaces(locationBlock);
	this->__line.erase(0, end);
}
void Template::addDirective(size_t end)
{
	String directive = String(__line.begin(), __line.begin() + end);
	WSU::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
}
void Template::parseDirectives()
{
	do
	{
		size_t pos = __line.find_first_of(";{");
		if (pos == String::npos && __line.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error("invalid block");
		if (pos == String::npos)
			break;
		if (__line.at(pos) == ';')
			addDirective(pos);
		else if (__line.at(pos) == '{')
			LocationBlock(pos);
	} while (true);
}
void Template::proccessLocations()
{
	String	outerScope = "locatio /";
}
void Template::parse()
{
	String line = __line;
	WSU::trimSpaces(__line);
	__line = __line.substr(1, __line.length() - 2);
	WSU::trimSpaces(__line);
	parseDirectives();
	proccessDirectives();
	this->__rootLocation.parseLocation(line);
}
