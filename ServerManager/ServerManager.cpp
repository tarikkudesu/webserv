
#include "ServerManager.hpp"

ServerManager::ServerManager()
{
	wsu::debug("ServerManager default constructor");
}

ServerManager::ServerManager(const String &configutation_file) : __config(configutation_file)
{
	wsu::debug("ServerManager single para constructor");
	wsu::info("configuration file: " + configutation_file);
}

ServerManager::ServerManager(const ServerManager &copy)
{
	wsu::debug("ServerManager copy constructor");
	*this = copy;
}

ServerManager &ServerManager::operator=(const ServerManager &assign)
{
	wsu::debug("ServerManager copy assignement operator");
	if (this != &assign)
	{
	}
	return *this;
}
ServerManager::~ServerManager()
{
	wsu::debug("ServerManager destructor");
	Core::clear();
}

/*************************************************************************
 *                             SERVER PARSER                             *
 *************************************************************************/
void ServerManager::checkHosts()
{
	for (t_serVect::iterator it = __serverTemplates.begin(); it != __serverTemplates.end(); it++)
	{
		Server *tmp = *it;
		if (!tmp->__valid)
			continue;
		const String &host = tmp->getServerHost();
		{
			struct addrinfo hint;
			struct addrinfo *result;
			memset(&hint, 0, sizeof(hint));
			hint.ai_family = AF_INET;
			hint.ai_socktype = SOCK_STREAM;
			hint.ai_protocol = IPPROTO_TCP;
			int status = getaddrinfo(host.c_str(), NULL, &hint, &result);
			if (status != 0)
				throw std::runtime_error("getaddrinfo: couldn't resolve server host name: " + host);
			freeaddrinfo(result);
		}
	}
	wsu::info("resolving hosts");
}
void ServerManager::initServers()
{
	for (t_serVect::iterator it = __serverTemplates.begin(); it != __serverTemplates.end(); it++)
	{
		Server *tmp = *it;
		if (!tmp->__valid)
			continue;
		std::vector<int> &ports = tmp->__ports;
		for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			Server *newServer = new Server(*tmp);
			newServer->setPort(*it);
			try
			{
				newServer->setup();
				Core::addServer(newServer);
			}
			catch (std::exception &e)
			{
				wsu::error(e.what());
			}
		}
	}
	for (t_serVect::iterator it = __serverTemplates.begin(); it != __serverTemplates.end(); it++)
		delete *it;
	__serverTemplates.clear();
}
void ServerManager::readFile()
{
	std::fstream fS;
	String line;

	fS.open(__config.c_str());
	if (!fS.is_open())
		throw std::runtime_error("coudln't open file");
	do
	{
		std::getline(fS, line, '\n');
		if (fS.fail())
			break;
		size_t pos = line.find("# ");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		if (line.empty() || String::npos == line.find_first_not_of(" \t\n\r\v\f"))
			continue;
		this->__lines.append(line);
		this->__lines.append(" ");
		line.clear();
		if (fS.eof())
			break;
	} while (true);
	fS.close();
	wsu::info("reading file content");
}
void ServerManager::firstCheck()
{
	if (__lines.empty() || String::npos == __lines.find_first_not_of(" \t\n\r\v\f"))
		throw std::runtime_error("empty file");
	if (String::npos == __lines.find_first_of("{}"))
		throw std::runtime_error("invalid config file");
	if (String::npos != __lines.find_first_not_of(PRINTABLE))
		throw std::runtime_error("unknown characters");
}
void ServerManager::checkBraces()
{
	size_t end = 0;
	size_t tracker = 0;
	do
	{
		if (end >= this->__lines.length())
			break;
		if (this->__lines.at(end) == '}')
			tracker--;
		if (this->__lines.at(end) == '{')
			tracker++;
		end++;
	} while (true);
	if (tracker != 0)
		throw std::runtime_error("unclosed curly braces");
}
void ServerManager::reduceSpaces()
{
	std::string result;
	bool inSpace = false;

	for (size_t i = 0; i < __lines.length(); i++)
	{
		if (std::isspace(__lines.at(i)))
		{
			if (!inSpace)
			{
				result += ' ';
				inSpace = true;
			}
		}
		else
		{
			result += __lines.at(i);
			inSpace = false;
		}
	}
	this->__lines.clear();
	this->__lines.append(result);
}
void ServerManager::setUpServer(size_t start)
{
	size_t end = start + 1;
	size_t tracker = 1;

	checkOuterscope(String(this->__lines.begin(), this->__lines.begin() + start));
	do
	{
		if (end >= this->__lines.length())
			break;
		if (this->__lines.at(end) == '}')
			tracker--;
		if (this->__lines.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	if (tracker != 0)
		throw std::runtime_error("unclosed curly braces");
	String serverConfig(this->__lines.begin() + start, this->__lines.begin() + end);
	this->__lines.erase(0, end);
	Server *server = new Server(serverConfig);
	__serverTemplates.push_back(server);
}
void ServerManager::setUpServers()
{
	do
	{
		size_t pos = this->__lines.find("{");
		if (pos == String::npos && __lines.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error("invalid config file");
		else if (pos == String::npos)
			break;
		setUpServer(pos);
	} while (!this->__lines.empty());
	wsu::info("syntax check");
}
void ServerManager::checkOuterscope(String outerScope)
{
	wsu::trimSpaces(outerScope);
	if (outerScope != "server")
		throw std::runtime_error("invalid config file");
	if (__lines.find_first_of("{}") == String::npos)
		throw std::runtime_error("invalid config file ");
}
void ServerManager::setUpWebserv()
{
	try
	{
		readFile();
		firstCheck();
		reduceSpaces();
		checkBraces();
		setUpServers();
		checkHosts();
		initServers();
		Core::checkConflicts();
		Core::logServers();
		Core::mainLoop();
	}
	catch (std::exception &e)
	{
		wsu::terr(e.what());
	}
}
