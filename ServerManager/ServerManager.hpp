#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Config.hpp"

typedef std::map< int, Connection* >		t_Connections;

void printLocalAddress(int sockfd);

class ServerManager
{
	private :
		int				__sockNum;
		t_events		__sockets;
		t_Server		__servers;
		t_Connections	__connections;
		String			__configFile;

		void	writeDataToSocket( int sd );
		void	readDataFromSocket( int sd );
		void	acceptNewConnection( int sd );
		void	proccessPollEvent( int retV );

		void	removeConnection( int sd );
		void	addConnection( int sd );
		void	removeServer( int sd );
		void	addServer( Server *server );
		void	removeSocket( int sd );
		void	addSocket( int sd, t_endian endian );

		bool	isServerSocket( int sd );

	public:
		ServerManager();
		ServerManager( const String &configutation_file );
		ServerManager( const ServerManager &copy );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();

		void	mainLoop();
		void	setUpWebserv();

		void	debug() {
			// t_Server::iterator it = this->__servers.begin();
			// for (; it != this->__servers.end(); it++) {
			// 	std::cout << "Socket: " << it->second->__sd << "\n";
			// 	std::cout << "Port: " << it->second->__port << "\n";
			// 	std::cout << "Name: " << it->second->__serverName << "\n";
			// }
			// removeServer(3);
			// it = this->__servers.begin();
			// for (; it != this->__servers.end(); it++) {
			// 	std::cout << "Socket: " << it->second->__sd << "\n";
			// 	std::cout << "Port: " << it->second->__port << "\n";
			// 	std::cout << "Name: " << it->second->__serverName << "\n";
			// }
			// this->addConnection(6);
			// this->addConnection(7);
			// this->addConnection(8);
			// this->addConnection(9);
			// t_Connections::iterator it = this->__connections.begin();
			// for (; it != this->__connections.end(); it++) {
			// 	std::cout << "Socket: " << it->second->__sd << "\n";
			// }
			t_events::iterator i = this->__sockets.begin();
			for (; i != this->__sockets.end(); i++) {
				std::cout << "Poll Socket: " << i->fd << "\n";
				printLocalAddress(i->fd);
			}
			// this->removeConnection(6);
			// this->removeConnection(7);
			// i = this->__sockets.begin();
			// for (; i != this->__sockets.end(); i++) {
			// 	std::cout << "Poll Socket: " << i->fd << "\n";
			// }
			// this->removeConnection(8);
			// this->removeConnection(9);
		};
};

#endif
