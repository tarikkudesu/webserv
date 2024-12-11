#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Config.hpp"

typedef std::map< int, Connection* >		t_Connections;

void printLocalAddress(int sockfd);

class ServerManager
{
	private :
		static int					__sockNum;
		static t_events				__sockets;
		static t_Connections		__connections;
		static t_Server				__servers;

		void				writeDataToSocket( int sd );
		void				readDataFromSocket( int sd );
		void				acceptNewConnection( int sd );
		void				proccessPollEvent( int retV );

		static void			removeConnection( int sd );
		static void			addConnection( int sd );
		static void			addSocket( int sd, t_endian endian );
		static void			removeSocket( int sd );
		static void			addServer( Server *server );
		static void			removeServer( int sd );

		static bool		isServerSocket( int sd );

	public:
		ServerManager();
		ServerManager( const String &configutation_file );
		ServerManager( const ServerManager &copy );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();

		void	mainLoop();
		void	setUpWebserv();

		void	debug() {
			t_Server::iterator it = ServerManager::__servers.begin();
			for (; it != ServerManager::__servers.end(); it++) {
				std::cout << "Socket: " << it->second->__sd << "\n";
				std::cout << "Port: " << it->second->__port << "\n";
				std::cout << "Name: " << it->second->__serverName << "\n";
			}
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
			// t_events::iterator i = this->__sockets.begin();
			// for (; i != this->__sockets.end(); i++) {
			// 	std::cout << "Poll Socket: " << i->fd << "\n";
			// 	printLocalAddress(i->fd);
			// }
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
