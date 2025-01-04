#include "Location.hpp"

std::map<int16_t, String> Location::__defaultErrorPages;

Location::Location()
{
	wsu::debug("Location default constructor");
}
Location::Location(const String &conf) : b__r(true),
										 __path("/"),
										 __line(conf),
										 __autoindex(false),
										 __clientBodyBufferSize(-1)
{
	wsu::debug("Location single para constructor");
	parse();
}
Location::Location(const String &dir, const String &conf) : b__r(false),
															__path(dir),
															__line(conf),
															__autoindex(false),
															__clientBodyBufferSize(-1)
{
	wsu::debug("Location double para constructor : " + dir);
	parse();
}
Location::Location(const Location &copy) : __path(copy.__path)
{
	wsu::debug("Location copy constructor");
	*this = copy;
}
Location &Location::operator=(const Location &assign)
{
	wsu::debug("Location copy assignement operator");
	if (this != &assign)
	{
		b__r = assign.b__r;
		__root = assign.__root;
		__index = assign.__index;
		__autoindex = assign.__autoindex;
		__errorPages = assign.__errorPages;
		__allowMethods = assign.__allowMethods;
		__clientBodyBufferSize = assign.__clientBodyBufferSize;
	}
	return *this;
}
Location::~Location()
{
	__directives.clear();
	__errorPages.clear();
	__allowMethods.clear();
	wsu::debug("Location destructor");
}
/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/
void Location::addErrPages()
{
	if (Location::__defaultErrorPages.empty())
	{
		__defaultErrorPages.insert(std::make_pair(300, "./Content/300.html"));
		__defaultErrorPages.insert(std::make_pair(301, "./Content/301.html"));
		__defaultErrorPages.insert(std::make_pair(302, "./Content/302.html"));
		__defaultErrorPages.insert(std::make_pair(303, "./Content/303.html"));
		__defaultErrorPages.insert(std::make_pair(304, "./Content/304.html"));
		__defaultErrorPages.insert(std::make_pair(305, "./Content/305.html"));
		__defaultErrorPages.insert(std::make_pair(307, "./Content/307.html"));
		__defaultErrorPages.insert(std::make_pair(400, "./Content/400.html"));
		__defaultErrorPages.insert(std::make_pair(401, "./Content/401.html"));
		__defaultErrorPages.insert(std::make_pair(402, "./Content/402.html"));
		__defaultErrorPages.insert(std::make_pair(403, "./Content/403.html"));
		__defaultErrorPages.insert(std::make_pair(404, "./Content/404.html"));
		__defaultErrorPages.insert(std::make_pair(405, "./Content/405.html"));
		__defaultErrorPages.insert(std::make_pair(406, "./Content/406.html"));
		__defaultErrorPages.insert(std::make_pair(407, "./Content/407.html"));
		__defaultErrorPages.insert(std::make_pair(408, "./Content/408.html"));
		__defaultErrorPages.insert(std::make_pair(409, "./Content/409.html"));
		__defaultErrorPages.insert(std::make_pair(410, "./Content/410.html"));
		__defaultErrorPages.insert(std::make_pair(411, "./Content/411.html"));
		__defaultErrorPages.insert(std::make_pair(412, "./Content/412.html"));
		__defaultErrorPages.insert(std::make_pair(413, "./Content/413.html"));
		__defaultErrorPages.insert(std::make_pair(414, "./Content/414.html"));
		__defaultErrorPages.insert(std::make_pair(415, "./Content/415.html"));
		__defaultErrorPages.insert(std::make_pair(416, "./Content/416.html"));
		__defaultErrorPages.insert(std::make_pair(417, "./Content/417.html"));
		__defaultErrorPages.insert(std::make_pair(500, "./Content/500.html"));
		__defaultErrorPages.insert(std::make_pair(501, "./Content/501.html"));
		__defaultErrorPages.insert(std::make_pair(502, "./Content/502.html"));
		__defaultErrorPages.insert(std::make_pair(503, "./Content/503.html"));
		__defaultErrorPages.insert(std::make_pair(504, "./Content/504.html"));
		__defaultErrorPages.insert(std::make_pair(505, "./Content/505.html"));
	}
}

/**************************************************************************************************************
 *                                           PROCCESSING DIRECTIVES                                           *
 **************************************************************************************************************/

void Location::proccessRootDirective(t_svec &tokens)
{
	if (!__root.empty())
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__root = tokens.at(1);
}
void Location::proccessIndexDirective(t_svec &tokens)
{
	for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		this->__index.push_back(*it);
}
void Location::proccessAutoindexDirective(t_svec &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	if (tokens.at(1) == "on")
		this->__autoindex = true;
	else if (tokens.at(1) != "off")
		throw std::runtime_error(tokens.at(0) + " invalid value, it must be \"on\" or \"off\"");
}
void Location::proccessErrorPageDirective(t_svec &tokens)
{
	if (tokens.size() <= 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end() && it != tokens.end() - 1; it++)
	{
		if (it->find_first_not_of("0123456789") != String::npos)
			throw std::runtime_error(*it + " invalid value");
		int code = wsu::stringToInt(*it);
		if (code < 300 || code > 599)
			throw std::runtime_error(tokens.at(0) + " value \"" + *it + "\" must be between 300 and 599");
		this->__errorPages.insert(std::make_pair(code, *(tokens.end() - 1)));
	}
}
void Location::proccessAllowMethodsDirective(t_svec &tokens)
{
	for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
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
void Location::proccessReturnDirective(t_svec &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__return = tokens.at(1);
}
void Location::proccessCgiPassDirective(t_svec &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__cgiPass = tokens.at(1);
}
void Location::proccessClientBodyBufferSizeToken(t_svec &tokens)
{
	if (this->__clientBodyBufferSize != -1)
		throw std::runtime_error(tokens.at(0) + " directive is duplicate");
	if (tokens.size() == 1)
		throw std::runtime_error(tokens.at(0) + ": no client_body_buffer_size value");
	if (tokens.size() > 2)
		throw std::runtime_error(tokens.at(0) + ": multiple client_body_buffer_size values");
	if (String::npos != tokens.at(1).find_first_not_of("0123456789"))
		throw std::runtime_error(tokens.at(0) + ": invalid client_body_buffer_size: not a number");
	this->__clientBodyBufferSize = wsu::stringToInt(tokens.at(1));
}
void Location::proccessToken(t_svec &tokens)
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
		key != "allow_methods" &&
		key != "client_body_buffer_size")
		throw std::runtime_error(key + ": invalid context");
	if (key == "root")
		proccessRootDirective(tokens);
	else if (key == "index")
		proccessIndexDirective(tokens);
	else if (key == "return")
		proccessReturnDirective(tokens);
	else if (key == "autoindex")
		proccessAutoindexDirective(tokens);
	else if (key == "cgi_pass")
		proccessCgiPassDirective(tokens);
	else if (key == "error_page")
		proccessErrorPageDirective(tokens);
	else if (key == "allow_methods")
		proccessAllowMethodsDirective(tokens);
	else if (key == "client_body_buffer_size")
		proccessClientBodyBufferSizeToken(tokens);
}
void Location::proccessDirectives()
{
	for (std::deque<String>::iterator it = this->__directives.begin(); it != this->__directives.end(); it++)
	{
		t_svec tokens = wsu::splitBySpaces(*it);
		if (!tokens.empty())
		{
			if (tokens.size() == 1)
				throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
			proccessToken(tokens);
		}
	}
}
/***************************************************************************************************************
 **************************************************** PARSE ****************************************************
 ***************************************************************************************************************/
void Location::LocationBlock(size_t pos)
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
void Location::addDirective(size_t end)
{
	String directive = String(__line.begin(), __line.begin() + end);
	wsu::trimSpaces(directive);
	if (directive.empty())
		throw std::runtime_error("empty directive");
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
	wsu::info("directive: " + directive);
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
			LocationBlock(pos);
	} while (true);
}
void Location::parse()
{
	addErrPages();
	wsu::trimSpaces(__line);
	if (__line.empty())
		throw std::runtime_error("empty location block");
	parseDirectives();
	proccessDirectives();
	if (__allowMethods.empty())
	{
		this->__allowMethods.push_back(GET);
		this->__allowMethods.push_back(DELETE);
		this->__allowMethods.push_back(POST);
	}
	if (__index.empty())
		this->__index.push_back("index.html");
	if (__clientBodyBufferSize == -1)
		__clientBodyBufferSize = 8000;
	this->__directives.clear();
	__line.clear();
}

String methodToString(t_method t)
{
	switch (t)
	{
		case GET:
			return "GET";
			break;
		case OPTIONS:
			return "OPTIONS";
			break;
		case HEAD:
			return "HEAD";
			break;
		case POST:
			return "POST";
			break;
		case PUT:
			return "PUT";
			break;
		case DELETE:
			return "DELETE";
			break;
		case TRACE:
			return "TRACE";
			break;
		case CONNECT:
			return "CONNECT";
			break;
		default:
			break;
	}
	return "NONE";
}

std::ostream &operator<<( std::ostream &o, const Location &loc )
{
	std::cout << "\tlocation: " << loc.__path << "\n";
	std::cout << "\t\troot: [" << loc.__root << "]\n";
	std::cout << "\t\tindex: ";
	for (t_svec::const_iterator it = loc.__index.begin(); it != loc.__index.end(); it++) {
		std::cout << *it << " ";
	} std::cout << "\n";
	std::cout << "\t\tautoindex: ";
	loc.__autoindex ? std::cout << "on\n" : std::cout << "off\n";
	std::cout << "\t\tcgi_pass: " << loc.__cgiPass << "\n";
	std::cout << "\t\treturn: " << loc.__return << "\n";
	std::cout << "\t\tallow_methods: ";
	for (std::vector< t_method >::const_iterator it = loc.__allowMethods.begin(); it != loc.__allowMethods.end(); it++) {
		std::cout << methodToString(*it) << " ";
	} std::cout << "\n";
	std::cout << "\t\terror_pages: ";
	for (std::map< int16_t, String >::const_iterator it = loc.__errorPages.begin(); it != loc.__errorPages.end(); it++) {
		std::cout << it->second << " ";
	} std::cout << "\n";
	std::cout << "\t\tclient_body_buffer_size: " << loc.__clientBodyBufferSize << "\n";
	return o;
}
