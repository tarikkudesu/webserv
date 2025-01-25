#include "Server.hpp"

Server::Server()
{
	wsu::debug("Server default constructor");
	throw std::runtime_error(": can not initiate server without port and server_name");
}
Server::Server(String line) : __sd(-1),
							  __port(-1),
							  __valid(false)
{
	wsu::debug("Server single para constructor");
	wsu::trimSpaces(line);
	if (!line.empty())
		line = line.substr(1, line.length() - 2);
	wsu::trimSpaces(line);
	parseServerDirectives(line);
	this->__locations.push_back(Location(line, this->__root));
	parseLocations(line, "/");
	if (__ports.size() == 0)
		__ports.push_back(8080);
	if (__host.empty())
		__host = "0.0.0.0";
	__directives.clear();
}
Server::Server(const Server &copy)
{
	wsu::debug("Server copy constructor");
	*this = copy;
}
Server &Server::operator=(const Server &assign)
{
	wsu::debug("Server copy assignement operator");
	if (this != &assign)
	{
		__sd = assign.__sd;
		__port = assign.__port;
		__host = assign.__host;
		__ports = assign.__ports;
		__locations = assign.__locations;
		__directives = assign.__directives;
		__serverNames = assign.__serverNames;
	}
	return *this;
}
Server::~Server()
{
	__ports.clear();
	__locations.clear();
	__directives.clear();
	__serverNames.clear();
	wsu::debug("Server destructor");
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
const String &Server::getServerHost() const
{
	return this->__host;
}
bool Server::amITheServerYouAreLookingFor(const String &sN)
{
	for (t_svec::iterator it = __serverNames.begin(); it != __serverNames.end(); it++)
	{
		if (*it == sN)
			return true;
	}
	return false;
}
String Server::serverIdentity() const
{
	return String(__host + ":" + wsu::intToString(__port));
}
Location &Server::identifyLocation(const String &URI)
{
	wsu::info("identifying location");
	for (std::vector<Location>::iterator it = __locations.begin(); it != __locations.end(); it++)
	{
		if (wsu::samePath(URI, it->__path))
			return *it;
	}
	std::vector<Location *> temp;
	for (std::vector<Location>::iterator it = __locations.begin(); it != __locations.end(); it++)
	{
		if (wsu::containsPath(it->__path, URI))
			temp.push_back(&*it);
	}
	Location *loc = &*__locations.begin();
	for (std::vector<Location *>::iterator it = temp.begin(); it != temp.end(); it++)
	{
		if (wsu::splitByChar((*it)->__path, '/').size() > wsu::splitByChar(loc->__path, '/').size())
			loc = *it;
	}
	return *loc;
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
			freeaddrinfo(result);
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

/**************************************************************************************************************
 *                                           PROCCESSING DIRECTIVES                                           *
 **************************************************************************************************************/

void Server::proccessRootToken(t_svec &tokens)
{
	if (!__root.empty())
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no root value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple root values");
	this->__root = tokens.at(1);
}
void Server::proccessHostToken(t_svec &tokens)
{
	if (!__host.empty())
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no host value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple host values");
	this->__host = tokens.at(1);
	this->__valid = true;
}
void Server::proccessListenToken(t_svec &tokens)
{
	if (tokens.size() > 80) // an extra leyer of protection, this value can be changed later
		throw std::runtime_error(tokens.at(0) + ": this amount of ports is excessive");
	else if (tokens.size() > 1)
	{
		for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
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
void Server::proccessServerNameToken(t_svec &tokens)
{
	if (tokens.size() > 20) // an extra leyer of protection, this value can be changed later
		throw std::runtime_error(tokens.at(0) + ": this amount of server names is excessive");
	else if (tokens.size() > 1)
	{
		for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
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

void Server::proccessToken(t_svec &tokens)
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
	else if (key == "root")
		proccessRootToken(tokens);
	else if (key == "listen")
		proccessListenToken(tokens);
	else if (key == "server_name")
		proccessServerNameToken(tokens);
}

/************************************************************************************************************
 ********************************************** LOCATION BLOCK **********************************************
 ************************************************************************************************************/

void Server::proccessLocationBlock(String &line, size_t pos, String &parent)
{
	t_svec tokens = wsu::splitBySpaces(String(line.begin(), line.begin() + pos));
	if (tokens.size() != 2)
		throw std::runtime_error("multiple directive");
	if (tokens.at(0) != "location")
		throw std::runtime_error("unknown block");
	if (wsu::containsPath(parent, tokens.at(1)) == false)
		throw std::runtime_error("location \"" + tokens.at(1) + "\" is outside location \"" + parent + "\"");
	for (std::vector<Location>::iterator it = __locations.begin(); it != __locations.end(); it++)
	{
		if (tokens.at(1) == "/")
			throw std::runtime_error("use server block to define root \"" + tokens.at(1) + "\" directives");
		if (wsu::samePath(it->__path, tokens.at(1)))
			throw std::runtime_error("duplicate location \"" + tokens.at(1) + "\"");
	}
	String conf = String(line.begin() + pos + 1, line.end() - 2);
	wsu::info("location: " + tokens.at(1));
	this->__locations.push_back(Location(tokens.at(1), conf, this->__root));
	parseLocations(conf, tokens.at(1));
}
void Server::addLocationBlock(String &line, size_t pos, String parent)
{
	size_t end = pos + 1;
	size_t tracker = 1;

	do
	{
		if (end >= line.length())
			break;
		if (line.at(end) == '}')
			tracker--;
		if (line.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	String locationBlock = String(line.begin(), line.begin() + end);
	proccessLocationBlock(locationBlock, pos, parent);
	line.erase(0, end);
}
void Server::parseLocations(String line, String parent)
{
	do
	{
		size_t pos = line.find_first_of(";{");
		if (pos == String::npos && line.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error("invalid block 1");
		if (pos == String::npos)
			break;
		if (line.at(pos) == ';')
			line.erase(0, pos + 1);
		else if (line.at(pos) == '{')
			addLocationBlock(line, pos, parent);
	} while (true);
}

/**********************************************************************************************************
 ********************************************** SERVER BLOCK **********************************************
 **********************************************************************************************************/

void Server::proccessServerDirectives()
{
	for (t_svec::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		t_svec tokens = wsu::splitBySpaces(*it);
		if (!tokens.empty())
		{
			proccessToken(tokens);
		}
	}
	if (this->__root.empty())
		this->__root = "./Content/";
}
void Server::skipLocationBlock(String &line, size_t pos)
{
	size_t end = pos + 1;
	size_t tracker = 1;
	do
	{
		if (end >= line.length())
			break;
		if (line.at(end) == '}')
			tracker--;
		if (line.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	line.erase(0, end);
}
void Server::addServerDirective(String &line, size_t end)
{
	String directive = String(line.begin(), line.begin() + end);
	wsu::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	line.erase(0, end + 1);
	wsu::info("directive: " + directive);
}
void Server::parseServerDirectives(String line)
{
	do
	{
		size_t pos = line.find_first_of(";{");
		if (pos == String::npos && line.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error("invalid block 3");
		if (pos == String::npos)
			break;
		if (line.at(pos) == ';')
			addServerDirective(line, pos);
		else if (line.at(pos) == '{')
			skipLocationBlock(line, pos);
	} while (true);
	proccessServerDirectives();
}

std::ostream &operator<<(std::ostream &o, const Server &ser)
{
	std::cout << "server: " << ser.serverIdentity() << "\n";
	std::cout << "\tserver_name: ";
	for (t_svec::const_iterator it = ser.__serverNames.begin(); it != ser.__serverNames.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << "\n";
	for (std::vector<Location>::const_iterator it = ser.__locations.begin(); it != ser.__locations.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << "\n";
	return o;
}
