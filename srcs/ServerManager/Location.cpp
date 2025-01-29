#include "Location.hpp"

Location::Location()
{
	wsu::debug("Location default constructor");
}
Location::Location(const String &conf, const String &root) : b__r(true),
															 __path("/"),
															 __line(conf),
															 __root(root),
															 __autoindex(false),
															 __clientBodyBufferSize(-1)
{
	wsu::debug("Location single para constructor");
	parse();
	wsu::resolvePath(__path);
	wsu::resolvePath(__root);
}
Location::Location(const String &dir, const String &conf, const String &root) : b__r(false),
																				__path(dir),
																				__line(conf),
																				__root(root),
																				__autoindex(false),
																				__clientBodyBufferSize(-1)
{
	wsu::debug("Location double para constructor : " + dir);
	parse();
	wsu::resolvePath(__path);
	wsu::resolvePath(__root);
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
		__return = assign.__return;
		__cgiPass = assign.__cgiPass;
		__autoindex = assign.__autoindex;
		__errorPages = assign.__errorPages;
		__allowMethods = assign.__allowMethods;
		__authenticate = assign.__authenticate;
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

/**************************************************************************************************************
 *											 PROCCESSING DIRECTIVES 										  *
 **************************************************************************************************************/

void Location::proccessRootDirective(t_svec &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__root = wsu::resolvePath(tokens.at(1));
}
void Location::proccessIndexDirective(t_svec &tokens)
{
	for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end(); it++)
		this->__index.push_back(wsu::resolvePath(*it));
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
void Location::proccessAuthenticateDirective(t_svec &tokens)
{
	if (tokens.size() != 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	this->__authenticate = tokens.at(1);
}
void Location::proccessErrorPageDirective(t_svec &tokens)
{
	if (tokens.size() <= 2)
		throw std::runtime_error(tokens.at(0) + " invalid number of arguments");
	String path = wsu::resolvePath(*(tokens.end() - 1));
	for (t_svec::iterator it = tokens.begin() + 1; it != tokens.end() && it != tokens.end() - 1; it++)
	{
		if (it->find_first_not_of("0123456789") != String::npos)
			throw std::runtime_error(*it + " invalid value");
		int code = wsu::stringToInt(*it);
		if (code < 300 || code > 599)
			throw std::runtime_error(tokens.at(0) + " value \"" + *it + "\" must be between 300 and 599");
		this->__errorPages.insert(std::make_pair(code, path));
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
		key != "authenticate" &&
		key != "allow_methods" &&
		key != "client_body_buffer_size")
		throw std::runtime_error(key + ": unknown directive");
	if (b__r == false &&
		key != "root" &&
		key != "index" &&
		key != "return" &&
		key != "cgi_pass" &&
		key != "autoindex" &&
		key != "error_page" &&
		key != "authenticate" &&
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
	else if (key == "authenticate")
		proccessAuthenticateDirective(tokens);
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
	wsu::trimSpaces(__line);
	if (__line.empty())
		throw std::runtime_error("empty location block");
	parseDirectives();
	proccessDirectives();
	for (std::map<int16_t, String>::iterator it = __errorPages.begin(); it != __errorPages.end(); it++)
		it->second = wsu::joinPaths(this->__root, it->second);
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
	if (t == GET)
		return "GET";
	else if (t == OPTIONS)
		return "OPTIONS";
	else if (t == HEAD)
		return "HEAD";
	else if (t == POST)
		return "POST";
	else if (t == PUT)
		return "PUT";
	else if (t == DELETE)
		return "DELETE";
	else if (t == TRACE)
		return "TRACE";
	else if (t == CONNECT)
		return "CONNECT";
	return "NONE";
}

std::ostream &operator<<(std::ostream &o, const Location &loc)
{
	std::cout << "\tlocation: " << loc.__path << "\n";
	std::cout << "\t\troot: [" << loc.__root << "]\n";
	std::cout << "\t\tindex: ";
	for (t_svec::const_iterator it = loc.__index.begin(); it != loc.__index.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << "\n";
	std::cout << "\t\tautoindex: ";
	loc.__autoindex ? std::cout << "on\n" : std::cout << "off\n";
	std::cout << "\t\tcgi_pass: " << loc.__cgiPass << "\n";
	std::cout << "\t\treturn: " << loc.__return << "\n";
	std::cout << "\t\tallow_methods: \n";
	std::cout << "\t\tauthenticate: " << loc.__authenticate << std::endl;
	for (std::vector<t_method>::const_iterator it = loc.__allowMethods.begin(); it != loc.__allowMethods.end(); it++)
	{
		std::cout << "\t\t" << methodToString(*it) << " ";
	}
	std::cout << "\n";
	std::cout << "\t\terror_pages: ";
	for (std::map<int16_t, String>::const_iterator it = loc.__errorPages.begin(); it != loc.__errorPages.end(); it++)
	{
		std::cout << it->second << " ";
	}
	std::cout << "\n";
	std::cout << "\t\tclient_body_buffer_size: " << loc.__clientBodyBufferSize << "\n";
	return o;
}
