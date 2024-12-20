#include "Location.hpp"

Location::Location() : b__r(true),
					   b__root(false),
					   b__index(false),
					   b__autoindex(false),
					   b__allowMethods(false),
					   __path("/"),
					   __autoindex(false)
{
	this->__allowMethods.push_back(GET);
	this->__allowMethods.push_back(DELETE);
	this->__allowMethods.push_back(POST);
	this->__index.push_back("index.html");
	addErrPages();
}
Location::Location(String dir) : b__r(false),
								 b__root(false),
								 b__index(false),
								 b__autoindex(false),
								 b__allowMethods(false),
								 __path(dir),
								 __autoindex(false)
{
	this->__allowMethods.push_back(GET);
	this->__allowMethods.push_back(DELETE);
	this->__allowMethods.push_back(POST);
	this->__index.push_back("index.html");
	addErrPages();
}
Location::Location(const Location &copy)
{
	*this = copy;
}
Location &Location::operator=(const Location &assign)
{
	if (this != &assign)
	{
		b__r = assign.b__r;
		__line = assign.__line;
		__root = assign.__root;
		__index = assign.__index;
		__autoindex = assign.__autoindex;
		__directives = assign.__directives;
		__errorPages = assign.__errorPages;
		__allowMethods = assign.__allowMethods;
		for (std::map<String, Location *>::const_iterator it = assign.__subLocations.begin(); it != assign.__subLocations.end(); it++)
		{
			String first = it->first;
			Location *second = new Location(*it->second);
			__subLocations.insert(std::make_pair(first, second));
		}
	}
	return *this;
}
Location::~Location()
{
	for (std::map<String, Location *>::iterator it = __subLocations.begin(); it != __subLocations.end(); it++)
	{
		delete it->second;
	}
	__subLocations.clear();
}
/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/
void Location::addErrPages()
{
	// __errorPages.insert(std::make_pair(300, "./Content/300.html"));
	// __errorPages.insert(std::make_pair(301, "./Content/301.html"));
	// __errorPages.insert(std::make_pair(302, "./Content/302.html"));
	// __errorPages.insert(std::make_pair(303, "./Content/303.html"));
	// __errorPages.insert(std::make_pair(304, "./Content/304.html"));
	// __errorPages.insert(std::make_pair(305, "./Content/305.html"));
	// __errorPages.insert(std::make_pair(307, "./Content/307.html"));
	// __errorPages.insert(std::make_pair(400, "./Content/400.html"));
	// __errorPages.insert(std::make_pair(401, "./Content/401.html"));
	// __errorPages.insert(std::make_pair(402, "./Content/402.html"));
	// __errorPages.insert(std::make_pair(403, "./Content/403.html"));
	// __errorPages.insert(std::make_pair(404, "./Content/404.html"));
	// __errorPages.insert(std::make_pair(405, "./Content/405.html"));
	// __errorPages.insert(std::make_pair(406, "./Content/406.html"));
	// __errorPages.insert(std::make_pair(407, "./Content/407.html"));
	// __errorPages.insert(std::make_pair(408, "./Content/408.html"));
	// __errorPages.insert(std::make_pair(409, "./Content/409.html"));
	// __errorPages.insert(std::make_pair(410, "./Content/410.html"));
	// __errorPages.insert(std::make_pair(411, "./Content/411.html"));
	// __errorPages.insert(std::make_pair(412, "./Content/412.html"));
	// __errorPages.insert(std::make_pair(413, "./Content/413.html"));
	// __errorPages.insert(std::make_pair(414, "./Content/414.html"));
	// __errorPages.insert(std::make_pair(415, "./Content/415.html"));
	// __errorPages.insert(std::make_pair(416, "./Content/416.html"));
	// __errorPages.insert(std::make_pair(417, "./Content/417.html"));
	// __errorPages.insert(std::make_pair(500, "./Content/500.html"));
	// __errorPages.insert(std::make_pair(501, "./Content/501.html"));
	// __errorPages.insert(std::make_pair(502, "./Content/502.html"));
	// __errorPages.insert(std::make_pair(503, "./Content/503.html"));
	// __errorPages.insert(std::make_pair(504, "./Content/504.html"));
	// __errorPages.insert(std::make_pair(505, "./Content/505.html"));
}
const String &Location::getPath() const
{
	return this->__path;
}
/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void Location::checkNestedLocation(String &path)
{
	if (wsu::containsPath(this->__path, path) == false)
		throw std::runtime_error("location \"" + path + "\" is outside location \"" + __path + "\"");
	for (std::map<String, Location *>::iterator it = __subLocations.begin(); it != __subLocations.end(); it++)
	{
		if (wsu::samePath(path, it->second->getPath()))
			throw std::runtime_error("duplicate location \"" + path + "\"");
	}
}
void Location::addLocationBlock(size_t pos)
{
	size_t end = pos + 1;
	size_t tracker = 1;

	String outer = String(__line.begin(), __line.begin() + pos);
	t_strVect tokens = wsu::splitBySpaces(outer);
	wsu::trimSpaces(outer);
	wsu::log("location: " + outer);
	if (tokens.size() != 2)
		throw std::runtime_error("location block error");
	if (tokens.at(0) != "location")
		throw std::runtime_error(tokens.at(0) + ": unknown block");
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
	String locationBlock(this->__line.begin() + pos, this->__line.begin() + end);
	wsu::trimSpaces(locationBlock);
	this->__line.erase(0, end);
	checkNestedLocation(tokens.at(1));
	Location *loc = new Location(tokens.at(1));
	loc->parseLocation(locationBlock);
	this->__subLocations.insert(std::make_pair(tokens.at(1), loc));
}
void Location::addDirective(size_t end)
{
	String directive = String(__line.begin(), __line.begin() + end);
	wsu::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
	wsu::log("directive: " + directive);
}
void Location::rootDirective(t_strVect &tokens)
{
	if (b__root == true)
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	b__root = true;
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__root = tokens.at(1);
}
void Location::indexDirective(t_strVect &tokens)
{
	if (b__index == false)
		this->__index.clear();
	b__index = true;
	for (t_strVect::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		this->__index.push_back(*it);
}
void Location::autoindexDirective(t_strVect &tokens)
{
	if (b__autoindex == true)
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	b__autoindex = true;
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	if (tokens.at(1) == "on")
		this->__autoindex = true;
	else if (tokens.at(1) != "off")
		throw std::runtime_error(tokens.at(0) + " invalid value, it must be \"on\" or \"off\"");
}
void Location::errorPageDirective(t_strVect &tokens)
{
	if (tokens.size() <= 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	for (t_strVect::iterator it = tokens.begin() + 1; it != tokens.end() && it != tokens.end() - 1; it++)
	{
		if (it->find_first_not_of("0123456789") != String::npos)
			throw std::runtime_error(*it + " invalid value");
		int code = wsu::stringToInt(*it);
		if (code < 300 || code > 599)
			throw std::runtime_error(tokens.at(0) + " value \"" + *it + "\" must be between 300 and 599");
		this->__errorPages.insert(std::make_pair(code, *(tokens.end() - 1)));
	}
}
void Location::allowMethodsDirective(t_strVect &tokens)
{
	if (b__allowMethods == false)
		this->__allowMethods.clear();
	b__allowMethods = true;
	for (t_strVect::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
	{
		if (*it != "OPTIONS" && *it != "GET" && *it != "HEAD" && *it != "PUT" && *it != "DELETE" && *it != "TRACE" && *it != "CONNECT" && *it != "POST")
			throw std::runtime_error(*it + " invalid value");
		if (*it == "OPTIONS" && std::find(__allowMethods.begin(), __allowMethods.end(), OPTIONS) == __allowMethods.end())
			this->__allowMethods.push_back(OPTIONS);
		else if (*it == "GET" && std::find(__allowMethods.begin(), __allowMethods.end(), GET) == __allowMethods.end())
			this->__allowMethods.push_back(GET);
		else if (*it == "HEAD" && std::find(__allowMethods.begin(), __allowMethods.end(), HEAD) == __allowMethods.end())
			this->__allowMethods.push_back(HEAD);
		else if (*it == "PUT" && std::find(__allowMethods.begin(), __allowMethods.end(), PUT) == __allowMethods.end())
			this->__allowMethods.push_back(PUT);
		else if (*it == "DELETE" && std::find(__allowMethods.begin(), __allowMethods.end(), DELETE) == __allowMethods.end())
			this->__allowMethods.push_back(DELETE);
		else if (*it == "TRACE" && std::find(__allowMethods.begin(), __allowMethods.end(), TRACE) == __allowMethods.end())
			this->__allowMethods.push_back(TRACE);
		else if (*it == "CONNECT" && std::find(__allowMethods.begin(), __allowMethods.end(), CONNECT) == __allowMethods.end())
			this->__allowMethods.push_back(CONNECT);
		else if (*it == "POST" && std::find(__allowMethods.begin(), __allowMethods.end(), POST) == __allowMethods.end())
			this->__allowMethods.push_back(POST);
	}
}
void Location::returnDirective(t_strVect &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__return = tokens.at(1);
}
void Location::cgiPassDirective(t_strVect &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__cgiPass = tokens.at(1);
}
void Location::proccessToken(t_strVect &tokens)
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
	if (b__r == false &&
		key != "root" &&
		key != "index" &&
		key != "autoindex" &&
		key != "error_page" &&
		key != "allow_methods")
		throw std::runtime_error(key + ": invalid context");
	if (key == "root")
		rootDirective(tokens);
	else if (key == "index")
		indexDirective(tokens);
	else if (key == "return")
		returnDirective(tokens);
	else if (key == "autoindex")
		autoindexDirective(tokens);
	else if (key == "cgi_pass")
		cgiPassDirective(tokens);
	else if (key == "error_page")
		errorPageDirective(tokens);
	else if (key == "allow_methods")
		allowMethodsDirective(tokens);
}
void Location::proccessDirectives()
{
	for (std::deque<String>::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		t_strVect tokens = wsu::splitBySpaces(*it);
		if (!tokens.empty())
		{
			if (tokens.size() == 1)
				throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
			proccessToken(tokens);
		}
	}
}
void Location::parseDirectives()
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
			addLocationBlock(pos);
	} while (true);
}
void Location::parseLocation(String conf)
{
	wsu::trimSpaces(conf);
	conf = conf.substr(1, conf.length() - 2);
	wsu::trimSpaces(conf);
	this->__line = conf;
	parseDirectives();
	proccessDirectives();
}
