#include "Server.hpp"

Server::Server()
{
	throw std::runtime_error(": can not initiate server without port and server_name");
}

Server::Server(String &line) : __sd(-1),
							   __port(-1),
							   __line(line),
							   __host("0.0.0.0"),
							   __clientBodyBufferSize(8000),
							   b__clientBodyBufferSize(false),
							   b__host(false),
							   __valid(false)
{
	parse();
}
Server::Server(const Server &copy)
{
	// wsu::log("Server copy constructor");
	*this = copy;
}

Server::~Server()
{
}

Server &Server::operator=(const Server &assign)
{
	if (this != &assign)
	{
		__sd = assign.__sd;
		__port = assign.__port;
		__line = assign.__line;
		__host = assign.__host;
		__ports = assign.__ports;
		__directives = assign.__directives;
		__serverNames = assign.__serverNames;
		__rootLocation = assign.__rootLocation;
		__clientBodyBufferSize = assign.__clientBodyBufferSize;
		b__clientBodyBufferSize = assign.b__clientBodyBufferSize;
		b__host = assign.b__host;
	}
	return *this;
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

int Server::getServerSocket() const
{
	return this->__sd;
}
int Server::getServerPort() const
{
	return this->__port;
}
void Server::setPort(int port)
{
	this->__port = port;
}
std::vector<int> &Server::getPorts()
{
	return this->__ports;
}
const String &Server::getServerHost() const
{
	return this->__host;
}
const t_strVect &Server::getServerNames() const
{
	return this->__serverNames;
}
bool Server::amITheServerYouAreLookingFor(const String &sN)
{
	for (t_strVect::iterator it = __serverNames.begin(); it != __serverNames.end(); it++)
	{
		if (*it == sN)
			return true;
	}
	return false;
}
String Server::serverIdentity()
{
	return String(__host + ":" + wsu::intToString(__port));
}
/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/

void Server::setup()
{
	struct sockaddr_in addr;
	int ra = 1;
	int rp = 1;

	this->__sd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->__sd == -1)
		throw std::runtime_error(serverIdentity() + ": non functional: failed to create socket ");
	if (-1 == setsockopt(__sd, SOL_SOCKET, SO_REUSEADDR, (void *)&ra, sizeof(ra)))
		throw std::runtime_error(serverIdentity() + ": non functional: failed to make reusable address");
	if (-1 == setsockopt(__sd, SOL_SOCKET, SO_REUSEPORT, (void *)&rp, sizeof(rp)))
		throw std::runtime_error(serverIdentity() + ": non functional: failed to make reusable port");
	addr.sin_family = AF_INET;
	{
		struct addrinfo hint;
		struct addrinfo *result;
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;
		hint.ai_socktype = SOCK_STREAM;
		int status = getaddrinfo(this->__host.c_str(), NULL, &hint, &result);
		if (status == 0)
		{
			struct sockaddr_in *add = (struct sockaddr_in *)result->ai_addr;
			addr.sin_addr.s_addr = add->sin_addr.s_addr;
		}
		else
			throw std::runtime_error(serverIdentity() + ": non functional: couldn't resolve server host name: " + this->__host);
	}
	addr.sin_port = htons(this->__port);
	if (-1 == bind(this->__sd, (struct sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error(serverIdentity() + ": non functional: failed to bind socket");
	if (-1 == listen(this->__sd, 10))
		throw std::runtime_error(serverIdentity() + ": non functional: failed to listen for connections");
}

/**********************************************************************
 *                            PARSE METHODS                           *
 **********************************************************************/
void Server::proccessHostToken(t_strVect &tokens)
{
	this->__host.clear();
	if (this->b__host)
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no host value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple host values");
	this->__host = tokens.at(1);
	this->b__host = true;
	this->__valid = true;
}
void Server::proccessListenToken(t_strVect &tokens)
{
	if (tokens.size() > 80) // an extra leyer of protection, this value can be changed later
		throw std::runtime_error(tokens.at(0) + ": this amount of ports is excessive");
	else if (tokens.size() > 1)
	{
		for (t_strVect::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (String::npos != it->find_first_not_of("0123456789"))
				throw std::runtime_error(tokens.at(0) + ": invalid port: not a number");
			size_t port = wsu::stringToInt(*it);
			if (port > 65535)
				throw std::runtime_error(tokens.at(0) + ": invalid port: out of range");
			this->__ports.push_back(port);
		}
	}
	else
		throw std::runtime_error(tokens.at(0) + ": no port value");
	this->__valid = true;
}
void Server::proccessServerNameToken(t_strVect &tokens)
{
	if (tokens.size() > 20) // an extra leyer of protection, this value can be changed later
		throw std::runtime_error(tokens.at(0) + ": this amount of server names is excessive");
	else if (tokens.size() > 1)
	{
		for (t_strVect::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (String::npos != it->find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_*"))
				throw std::runtime_error(tokens.at(0) + ": invalid server_name: out of character range");
			this->__serverNames.push_back(*it);
		}
	}
	else
		throw std::runtime_error(tokens.at(0) + ": no server_name value");
	this->__valid = true;
}
void Server::proccessClientBodyBufferSizeToken(t_strVect &tokens)
{
	if (this->b__clientBodyBufferSize)
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no client_body_buffer_size value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple client_body_buffer_size values");
	if (String::npos != tokens.at(1).find_first_not_of("0123456789"))
		throw std::runtime_error(tokens.at(0) + ": invalid client_body_buffer_size: not a number");
	this->__clientBodyBufferSize = wsu::stringToInt(tokens.at(1));
	this->b__clientBodyBufferSize = true;
}

void Server::proccessToken(t_strVect &tokens)
{
	String &key = tokens.at(0);
	if (key != "host" &&
		key != "root" &&
		key != "index" &&
		key != "listen" &&
		key != "return" &&
		key != "cgi_pass" &&
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
void Server::proccessDirectives()
{
	for (t_strVect::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		t_strVect tokens = wsu::splitBySpaces(*it);
		if (!tokens.empty())
		{
			proccessToken(tokens);
		}
	}
}
void Server::LocationBlock(size_t pos)
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
	this->__line.erase(0, end);
}
void Server::addDirective(size_t end)
{
	String directive = String(__line.begin(), __line.begin() + end);
	wsu::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
	wsu::log("directive: " + directive);
}
void Server::parseDirectives()
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
void Server::parse()
{
	String line = __line;
	wsu::trimSpaces(__line);
	if (!__line.empty())
		__line = __line.substr(1, __line.length() - 2);
	wsu::trimSpaces(__line);
	parseDirectives();
	proccessDirectives();
	if (__valid)
		__rootLocation.parseLocation(line);
	if (__ports.size() == 0)
		__ports.push_back(8080);
}
