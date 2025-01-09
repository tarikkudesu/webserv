#include "Core.hpp"

Core::Core()
{
    wsu::debug("core constructor");
}
Core::Core( const Core &copy )
{
    wsu::debug("copy constructor");
    (void)copy;
}
Core	&Core::operator=( const Core &assign )
{
    wsu::debug("copy assignement operator");
	(void)assign;
    return *this;
}
Core::~Core()
{
    clear();
    wsu::debug("core destructor");
}

bool Core::up = true;
int Core::__sockNum = 0;
t_Server Core::__servers;
t_events Core::__sockets;
t_Connections Core::__connections;
struct pollfd *Core::__events = NULL;

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void Core::clear()
{
    wsu::debug("clearing data");
	if (Core::__connections.empty() == false)
	{
		for (t_Connections::iterator it = Core::__connections.begin(); it != Core::__connections.end(); it++)
		{
			Core::removeSocket(it->second->getSock());
			delete it->second;
		}
	}
	if (Core::__servers.empty() == false)
	{
		for (t_Server::iterator it = Core::__servers.begin(); it != Core::__servers.end(); it++)
		{
			Core::removeSocket(it->second->getServerSocket());
			delete it->second;
		}
	}
	if (Core::__sockets.empty() == false)
	{
		for (t_events::iterator it = Core::__sockets.begin(); it != Core::__sockets.end(); it++)
		{
			close(it->fd);
		}
		Core::__sockets.clear();
	}
	Core::__sockNum = 0;
	Core::__servers.clear();
	Core::__sockets.clear();
	Core::__connections.clear();
}
void Core::removeConnection(int sd)
{
	t_Connections::iterator it = Core::__connections.find(sd);
	if (it != Core::__connections.end())
	{
		Connection *instance = it->second;
		Core::__connections.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void Core::addConnection(int sd)
{
	Core::__connections[sd] = new Connection(sd);
	Core::__connections[sd]->setServers(Core::__servers);
	addSocket(sd, CONNECTION);
}
void Core::removeServer(int sd)
{
	t_Server::iterator it = Core::__servers.find(sd);
	if (it != Core::__servers.end())
	{
		Server *instance = it->second;
		Core::__servers.erase(it);
		delete instance;
		removeSocket(sd);
	}
}
void Core::addServer(Server *server)
{
	if (Core::__servers.size() >= MAX_EVENTS)
		throw std::runtime_error("critical server overload, " \
			+ server->getServerHost() + ":" + wsu::intToString(server->getServerPort()) \
			+ " non functional");
	Core::__servers[server->getServerSocket()] = server;
	addSocket(server->getServerSocket(), SERVER);
}
void Core::removeSocket(int sd)
{
	for (t_events::iterator it = __sockets.begin(); it != __sockets.end(); it++)
	{
		if (sd == it->fd)
		{
			Core::__sockets.erase(it);
			Core::__sockNum--;
			close(sd);
			return;
		}
	}
}
void Core::addSocket(int sd, t_endian endian)
{
	struct pollfd sockStruct;

	sockStruct.fd = sd;
	if (endian == SERVER)
		sockStruct.events = POLLIN;
	else if (endian == CONNECTION)
		sockStruct.events = POLLIN | POLLOUT | POLLHUP;
	wsu::setNonBlockingMode(sd);
	Core::__sockets.push_back(sockStruct);
	Core::__sockNum++;
}
bool Core::isServerSocket(int sd)
{
	if (Core::__servers.find(sd) != Core::__servers.end())
		return true;
	return false;
}

void Core::logServers()
{
	if (Core::__servers.empty())
		throw std::runtime_error("config file does not identify any server");
	t_Server::iterator it = Core::__servers.begin();
	for (; it != Core::__servers.end(); it++)
	{
		wsu::running((*it).second->getServerHost() + ":" + wsu::intToString((*it).second->getServerPort()));
	}
}
void Core::checkConflicts()
{
	for (t_Server::iterator it = Core::__servers.begin(); it != Core::__servers.end(); it++)
	{
		const t_svec &serverNames = it->second->__serverNames;
		for (t_svec::const_iterator name = serverNames.begin(); name != serverNames.end(); name++)
		{
			for (t_svec::const_iterator match = name + 1; match != serverNames.end(); match++)
			{
				if (*match == *name)
					wsu::warn("conflicting server name \"" + *name + "\" on " + it->second->serverIdentity() + ", ignored");
			}
		}
	}
	for (t_Server::iterator it = Core::__servers.begin(); it != Core::__servers.end(); it++)
	{
		for (t_Server::iterator iter = Core::__servers.begin(); iter != it && iter != Core::__servers.end(); iter++)
		{
			if (it->second->getServerPort() == iter->second->getServerPort())
			{
				const t_svec &serverNames = it->second->__serverNames;
				for (t_svec::const_iterator name = serverNames.begin(); name != serverNames.end(); name++)
				{
					if (iter->second->amITheServerYouAreLookingFor(*name))
						wsu::warn("conflicting server name \"" + *name + "\" on " + it->second->serverIdentity() + ", ignored");
				}
			}
		}
	}
}

/************************************************************************
 *                            SERVER CONTROL                            *
 ************************************************************************/

void Core::writeDataToSocket(int sd)
{
	if (wsu::__criticalOverLoad == true && Core::__connections[sd]->__responseQueue.empty())
	{
		Core::removeConnection(sd);
		return;
	}
	if (Core::__connections[sd]->__responseQueue.empty())
		return;
	String response = Core::__connections[sd]->__responseQueue.front();
	Core::__connections[sd]->__responseQueue.pop();
	ssize_t bytesWritten = send(sd, response.c_str(), strlen(response.c_str()), 0);
	if (bytesWritten > 0)
	{
		wsu::info("response sent");
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) != 0 ||
			sockErr != 0)
		{
			removeConnection(sd);
			wsu::info("removing connection");
		}
	}
}
void Core::readDataFromSocket(int sd)
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
		t_Connections::iterator iter = Core::__connections.find(sd);
		if (iter != Core::__connections.end())
		{
			wsu::info("receiving data");
			iter->second->proccessData(String(buff));
		}
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) != 0 ||
			sockErr != 0) // surprise
		{
			removeConnection(sd);
			wsu::info("removing connection");
		}
	}
}

void Core::acceptNewConnection(int sd)
{
	int newSock;

	if (wsu::__criticalOverLoad == true)
		return;
	newSock = accept(sd, NULL, NULL);
	if (newSock >= 0)
	{
		wsu::info("accepting new connection");
		addConnection(newSock);
	}
	else
	{
		int sockErr = 0;
		if (setsockopt(sd, SOL_SOCKET, SO_ERROR,
					   &sockErr, sizeof(sockErr)) != 0 ||
			sockErr != 0)
		{
			removeServer(sd);
		}
	}
}
void Core::proccessPollEvent(int sd, int &retV)
{
	struct pollfd &sockStruct = Core::__events[sd];
	if (sockStruct.revents & POLLIN)
	{
		if (isServerSocket(sockStruct.fd))
		{
			if (Core::__sockets.size() >= MAX_EVENTS)
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
		wsu::info("removing connection");
		retV--;
	}
	else if (wsu::__criticalOverLoad == true)
	{
		wsu::fatal("critcal server overload");
		if (!Core::isServerSocket(sockStruct.fd))
		{
			removeConnection(sockStruct.fd);
			wsu::info("removing connection");
		}
	}
	if (Core::__servers.size() == Core::__sockets.size())
		wsu::__criticalOverLoad = false;
}

/***************************************************************************************
 *                                      MAIN LOOP                                      *
 ***************************************************************************************/

void Core::mainLoop()
{
	int retV = 0;
	int timeout = 10;

	try
	{
		while (up)
		{
			Core::__events = wsu::data(Core::__sockets);
			retV = poll(Core::__events, Core::__sockets.size(), timeout);
			if (retV != 0)
			{
				for (int sd = 0; sd < Core::__sockNum && retV; sd++)
				{
					if (wsu::__criticalOverLoad == true)
						retV = Core::__sockNum;
					try
					{
						proccessPollEvent(sd, retV);
					}
					catch (std::exception &e)
					{
						wsu::terr(e.what());
					}
				}
			}
			delete[] Core::__events;
		}
	}
	catch (std::exception &e)
	{
		wsu::terr(e.what());
	}
}
