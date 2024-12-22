
#include "ServerManager.hpp"

int ServerManager::__sockNum = 0;
t_Server ServerManager::__servers;
t_events ServerManager::__sockets;
t_Connections ServerManager::__connections;
struct pollfd *ServerManager::__events = NULL;

ServerManager::ServerManager()
{
}

ServerManager::ServerManager(const String &configutation_file) : __config(configutation_file)
{
	wsu::log("configuration file:" + configutation_file);
}

ServerManager::ServerManager(const ServerManager &copy)
{
	*this = copy;
}

ServerManager &ServerManager::operator=(const ServerManager &assign)
{
	if (this != &assign)
	{
	}
	return *this;
}

void ServerManager::clear()
{
	if (ServerManager::__connections.empty() == false)
	{
		for (t_Connections::iterator it = ServerManager::__connections.begin(); it != ServerManager::__connections.end(); it++)
		{
			ServerManager::removeSocket(it->second->getSock());
			delete it->second;
		}
	}
	if (ServerManager::__servers.empty() == false)
	{
		for (t_Server::iterator it = ServerManager::__servers.begin(); it != ServerManager::__servers.end(); it++)
		{
			ServerManager::removeSocket(it->second->getServerSocket());
			delete it->second;
		}
	}
	if (ServerManager::__sockets.empty() == false)
	{
		for (t_events::iterator it = ServerManager::__sockets.begin(); it != ServerManager::__sockets.end(); it++)
		{
			close(it->fd);
		}
		ServerManager::__sockets.clear();
	}
}

ServerManager::~ServerManager()
{
	// if (ServerManager::__sockets.empty() == false)
	// {
	// 	for (t_events::iterator it = ServerManager::__sockets.begin(); it != ServerManager::__sockets.end(); it++)
	// 	{
	// 		std::cout << it->fd << " |\n";
	// 		close(it->fd);
	// 	}
	// 	ServerManager::__sockets.clear();
	// }
	// if (ServerManager::__connections.empty() == false)
	// {
	// 	for (t_Connections::iterator it = ServerManager::__connections.begin(); it != ServerManager::__connections.end(); it++)
	// 	{
	// 		std::cout << it->second->getSock();
	// 	}
	// 	ServerManager::__connections.clear();
	// }
	// if (ServerManager::__servers.empty() == false)
	// {
	// 	for (t_Server::iterator it = ServerManager::__servers.begin(); it != ServerManager::__servers.end(); it++)
	// 	{
	// 		std::cout << it->second->getServerSocket();
	// 	}
	// 	ServerManager::__servers.clear();
	// }
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void ServerManager::removeConnection(int sd)
{
	t_Connections::iterator it = ServerManager::__connections.find(sd);
	if (it != ServerManager::__connections.end())
	{
		Connection *instance = it->second;
		ServerManager::__connections.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void ServerManager::addConnection(int sd)
{
	ServerManager::__connections[sd] = new Connection(sd);
	ServerManager::__connections[sd]->setServers(ServerManager::__servers);
	std::cout << "socket " << sd << " connection\n";
	addSocket(sd, CONNECTION);
}
void ServerManager::removeServer(int sd)
{
	t_Server::iterator it = ServerManager::__servers.find(sd);
	if (it != ServerManager::__servers.end())
	{
		Server *instance = it->second;
		ServerManager::__servers.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void ServerManager::addServer(Server *server)
{
	if (ServerManager::__servers.size() >= MAX_EVENTS)
		throw std::runtime_error("critical server overload, " + server->getServerHost() + ":" + wsu::intToString(server->getServerPort()) + " non functional");
	ServerManager::__servers[server->getServerSocket()] = server;
	std::cout << "socket " << server->getServerSocket() << " server\n";
	addSocket(server->getServerSocket(), SERVER);
}
void ServerManager::removeSocket(int sd)
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++)
	{
		if (sd == it->fd)
		{
			ServerManager::__sockets.erase(it);
			ServerManager::__sockNum--;
			close(sd);
			return;
		}
	}
}
void ServerManager::addSocket(int sd, t_endian endian)
{
	struct pollfd sockStruct;

	sockStruct.fd = sd;
	if (endian == SERVER)
		sockStruct.events = POLLIN;
	else if (endian == CONNECTION)
		sockStruct.events = POLLIN | POLLOUT | POLLHUP;
	wsu::setNonBlockingMode(sd);
	ServerManager::__sockets.push_back(sockStruct);
	ServerManager::__sockNum++;
}
bool ServerManager::isServerSocket(int sd)
{
	if (ServerManager::__servers.find(sd) != ServerManager::__servers.end())
		return true;
	return false;
}
/************************************************************************
 *                            SERVER CONTROL                            *
 ************************************************************************/
void ServerManager::writeDataToSocket(int sd)
{
	if (wsu::__criticalOverLoad == true && ServerManager::__connections[sd]->__responseQueue.empty())
	{
		ServerManager::removeConnection(sd);
		return;
	}
	if (ServerManager::__connections[sd]->__responseQueue.empty())
		return;
	String response = ServerManager::__connections[sd]->__responseQueue.front();
	ServerManager::__connections[sd]->__responseQueue.pop();
	ssize_t bytesWritten = send(sd, response.c_str(), strlen(response.c_str()), 0);
	if (bytesWritten > 0)
	{
		wsu::log("send " + wsu::intToString(sd));
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			removeConnection(sd);
			wsu::log("remove " + wsu::intToString(sd));
		}
	}
}
void ServerManager::readDataFromSocket(int sd)
{
	char buff[READ_SIZE + 1];

	ssize_t bytesRead = recv(sd, buff, READ_SIZE, 0);
	if (bytesRead == 0)
	{
		removeConnection(sd);
	}
	else if (bytesRead > 0)
	{
		buff[bytesRead] = '\0';
		t_Connections::iterator iter = ServerManager::__connections.find(sd);
		if (iter != ServerManager::__connections.end())
		{
			wsu::log("recv " + wsu::intToString(sd));
			iter->second->proccessData(String(buff));
		}
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			removeConnection(sd);
			wsu::log("remove " + wsu::intToString(sd));
		}
	}
}

void ServerManager::acceptNewConnection(int sd)
{
	int newSock;

	if (wsu::__criticalOverLoad == true)
		return;
	newSock = accept(sd, NULL, NULL);
	if (newSock >= 0)
	{
		wsu::log("accept " + wsu::intToString(sd));
		addConnection(newSock);
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) == 0 &&
			sockErr == 0)
		{
			return;
		}
		else
		{
			removeServer(sd);
		}
	}
}
void ServerManager::proccessPollEvent(int sd, int &retV)
{
	struct pollfd &sockStruct = ServerManager::__events[sd];
	if (sockStruct.revents & POLLIN)
	{
		if (isServerSocket(sockStruct.fd))
		{
			if (ServerManager::__sockets.size() >= MAX_EVENTS)
				wsu::__criticalOverLoad = true;
			else
			{
				acceptNewConnection(sockStruct.fd);
				retV--;
			}
		}
		else
		{
			readDataFromSocket(sockStruct.fd);
			retV--;
		}
	}
	else if (sockStruct.revents & POLLOUT)
	{
		writeDataToSocket(sockStruct.fd);
		retV--;
	}
	else if (sockStruct.revents & POLLHUP)
	{
		removeConnection(sockStruct.fd);
		retV--;
	}
	else if (wsu::__criticalOverLoad == true)
	{
		wsu::warn("critcal server overload");
		if (!ServerManager::isServerSocket(sockStruct.fd))
		{
			removeConnection(sockStruct.fd);
			wsu::log("remove " + wsu::intToString(sockStruct.fd));
		}
	}
	if (ServerManager::__servers.size() == ServerManager::__sockets.size())
		wsu::__criticalOverLoad = false;
}
/***************************************************************************************
 *                                      MAIN LOOP                                      *
 ***************************************************************************************/
void ServerManager::mainLoop()
{
	int retV = 0;
	int timeout = 1000;

	try
	{
		while (true)
		{
			ServerManager::__events = wsu::data(ServerManager::__sockets);
			retV = poll(ServerManager::__events, ServerManager::__sockets.size(), timeout);
			if (retV == -1)
			{
				delete[] ServerManager::__events;
			}
			else if (retV == 0)
			{
				delete[] ServerManager::__events;
				continue;
			}
			else
			{
				for (int sd = 0; sd < ServerManager::__sockNum && retV; sd++)
				{
					if (wsu::__criticalOverLoad == true)
						retV = ServerManager::__sockNum;
					try
					{
						this->proccessPollEvent(sd, retV);
					}
					catch (std::exception &e)
					{
						wsu::terr(e.what());
					}
				}
				delete[] ServerManager::__events;
			}
		}
	}
	catch (std::exception &e)
	{
		wsu::terr(e.what());
	}
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
			int status = getaddrinfo(host.c_str(), NULL, &hint, &result);
			if (status != 0)
			{
				freeaddrinfo(result);
				throw std::runtime_error("getaddrinfo: couldn't resolve server host name: " + host);
			}
			freeaddrinfo(result);
		}
	}
	wsu::success("resolving hosts");
}
void ServerManager::initServers()
{
	for (t_serVect::iterator it = __serverTemplates.begin(); it != __serverTemplates.end(); it++)
	{
		Server *tmp = *it;
		if (!tmp->__valid)
			continue;
		std::vector<int> &ports = tmp->getPorts();
		for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			Server *newServer = new Server(*tmp);
			newServer->setPort(*it);
			try
			{
				newServer->setup();
				ServerManager::addServer(newServer);
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
	if (ServerManager::__servers.size() >= MAX_EVENTS)
		throw std::runtime_error("critical server overload: too many servers");
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
		if (line.empty() || String::npos == line.find_first_not_of(" \t\n\r\v\f"))
			continue;
		this->__lines.append(line);
		this->__lines.append(" ");
		wsu::log(line);
		line.clear();
		if (fS.eof())
			break;
	} while (true);
	fS.close();
	wsu::success("reading file content");
}
void ServerManager::firstCheck()
{
	if (__lines.empty() || String::npos == __lines.find_first_not_of(" \t\n\r\v\f"))
		throw std::runtime_error("empty file");
	if (String::npos == __lines.find_first_of("{}"))
		throw std::runtime_error("invalid config file 1");
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
			throw std::runtime_error("invalid config file 4");
		else if (pos == String::npos)
			break;
		setUpServer(pos);
	} while (!this->__lines.empty());
	wsu::success("syntax check");
}
void ServerManager::checkOuterscope(String outerScope)
{
	wsu::trimSpaces(outerScope);
	if (outerScope != "server")
		throw std::runtime_error("invalid config file 2");
	if (__lines.find_first_of("{}") == String::npos)
		throw std::runtime_error("invalid config file 3");
}
void ServerManager::logServers()
{
	if (ServerManager::__servers.empty())
		throw std::runtime_error("config file does not identify any server");
	t_Server::iterator it = ServerManager::__servers.begin();
	for (; it != ServerManager::__servers.end(); it++)
	{
		wsu::running("Server: " + wsu::intToString(it->second->getServerSocket()) + " " + (*it).second->getServerHost() + ":" + wsu::intToString((*it).second->getServerPort()));
	}
}
void ServerManager::checkConflicts()
{
	for (t_Server::iterator it = ServerManager::__servers.begin(); it != ServerManager::__servers.end(); it++)
	{
		const t_svec &serverNames = it->second->getServerNames();
		for (t_svec::const_iterator name = serverNames.begin(); name != serverNames.end(); name++)
		{
			for (t_svec::const_iterator match = name + 1; match != serverNames.end(); match++)
			{
				if (*match == *name)
					wsu::warn("conflicting server name " + *name + " on " + it->second->serverIdentity() + ", ignored");
			}
		}
	}
	for (t_Server::iterator it = ServerManager::__servers.begin(); it != ServerManager::__servers.end(); it++)
	{
		for (t_Server::iterator iter = ServerManager::__servers.begin(); iter != it && iter != ServerManager::__servers.end(); iter++)
		{
			if (it->second->getServerPort() == iter->second->getServerPort())
			{
				const t_svec &serverNames = it->second->getServerNames();
				for (t_svec::const_iterator name = serverNames.begin(); name != serverNames.end(); name++)
				{
					if (iter->second->amITheServerYouAreLookingFor(*name))
						wsu::warn("conflicting server name " + *name + " on " + it->second->serverIdentity() + ", ignored");
				}
			}
		}
	}
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
		checkConflicts();
		while (1);
		logServers();
		mainLoop();
	}
	catch (std::exception &e)
	{
		wsu::terr(e.what());
	}
}
