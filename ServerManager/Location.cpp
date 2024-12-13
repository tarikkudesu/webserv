#include "Location.hpp"

Location::Location() : __r(true),
					   __root("/"),
					   __index("index.html"),
					   __dirListing(false)
{
	this->__allowMethods.push_back(GET);
	this->__allowMethods.push_back(DELETE);
	this->__allowMethods.push_back(POST);
	addErrPages();
}
Location::Location(String dir) : __r(false),
								 __root(dir),
								 __index("index.html"),
								 __dirListing(false)
{
	this->__allowMethods.push_back(GET);
	this->__allowMethods.push_back(DELETE);
	this->__allowMethods.push_back(POST);
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
		__line = assign.__line;
		__root = assign.__root;
		__index = assign.__index;
		__dirListing = assign.__dirListing;
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
	__errorPages.insert(std::make_pair(100, "./Content/100.html"));
	__errorPages.insert(std::make_pair(101, "./Content/101.html"));
	__errorPages.insert(std::make_pair(200, "./Content/200.html"));
	__errorPages.insert(std::make_pair(201, "./Content/201.html"));
	__errorPages.insert(std::make_pair(202, "./Content/202.html"));
	__errorPages.insert(std::make_pair(203, "./Content/203.html"));
	__errorPages.insert(std::make_pair(204, "./Content/204.html"));
	__errorPages.insert(std::make_pair(205, "./Content/205.html"));
	__errorPages.insert(std::make_pair(206, "./Content/206.html"));
	__errorPages.insert(std::make_pair(300, "./Content/300.html"));
	__errorPages.insert(std::make_pair(301, "./Content/301.html"));
	__errorPages.insert(std::make_pair(302, "./Content/302.html"));
	__errorPages.insert(std::make_pair(303, "./Content/303.html"));
	__errorPages.insert(std::make_pair(304, "./Content/304.html"));
	__errorPages.insert(std::make_pair(305, "./Content/305.html"));
	__errorPages.insert(std::make_pair(307, "./Content/307.html"));
	__errorPages.insert(std::make_pair(400, "./Content/400.html"));
	__errorPages.insert(std::make_pair(401, "./Content/401.html"));
	__errorPages.insert(std::make_pair(402, "./Content/402.html"));
	__errorPages.insert(std::make_pair(403, "./Content/403.html"));
	__errorPages.insert(std::make_pair(404, "./Content/404.html"));
	__errorPages.insert(std::make_pair(405, "./Content/405.html"));
	__errorPages.insert(std::make_pair(406, "./Content/406.html"));
	__errorPages.insert(std::make_pair(407, "./Content/407.html"));
	__errorPages.insert(std::make_pair(408, "./Content/408.html"));
	__errorPages.insert(std::make_pair(409, "./Content/409.html"));
	__errorPages.insert(std::make_pair(410, "./Content/410.html"));
	__errorPages.insert(std::make_pair(411, "./Content/411.html"));
	__errorPages.insert(std::make_pair(412, "./Content/412.html"));
	__errorPages.insert(std::make_pair(413, "./Content/413.html"));
	__errorPages.insert(std::make_pair(414, "./Content/414.html"));
	__errorPages.insert(std::make_pair(415, "./Content/415.html"));
	__errorPages.insert(std::make_pair(416, "./Content/416.html"));
	__errorPages.insert(std::make_pair(417, "./Content/417.html"));
	__errorPages.insert(std::make_pair(500, "./Content/500.html"));
	__errorPages.insert(std::make_pair(501, "./Content/501.html"));
	__errorPages.insert(std::make_pair(502, "./Content/502.html"));
	__errorPages.insert(std::make_pair(503, "./Content/503.html"));
	__errorPages.insert(std::make_pair(504, "./Content/504.html"));
	__errorPages.insert(std::make_pair(505, "./Content/505.html"));
}
/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void Location::addLocationBlock(size_t pos)
{
	size_t end = pos + 1;
	size_t tracker = 1;

	String outer = String(__line.begin(), __line.begin() + pos);
	std::vector<String> tokens = WSU::splitBySpaces(outer);
	WSU::trimSpaces(outer);
	WSU::log("location: " + outer);
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
	WSU::trimSpaces(locationBlock);
	this->__line.erase(0, end);
	Location *loc = new Location(tokens.at(1));
	loc->parseLocation(locationBlock);
	this->__subLocations.insert(std::make_pair(tokens.at(1), loc));
}
void Location::addDirective(size_t end)
{
	String directive = String(__line.begin(), __line.begin() + end);
	WSU::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
	WSU::log("directive: " + directive);
}
void Location::proccessToken(std::vector<String> &tokens)
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
	if (__r == false &&
		key != "root" &&
		key != "index" &&
		key != "autoindex" &&
		key != "error_page" &&
		key != "allow_methods")
		throw std::runtime_error(key + ": invalid context");
}
void Location::proccessDirectives()
{
	for (std::deque<String>::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		std::vector<String> tokens = WSU::splitBySpaces(*it);
		if (!tokens.empty())
			proccessToken(tokens);
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
	WSU::trimSpaces(conf);
	conf = conf.substr(1, conf.length() - 2);
	WSU::trimSpaces(conf);
	this->__line = conf;
	parseDirectives();
	proccessDirectives();
}
