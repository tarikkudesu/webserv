#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Connection.hpp"

typedef std::map< int, Connection* >		t_Connections;

void printLocalAddress(int sockfd);

class ServerManager
{
	private :
		String						__lines;
		String						__config;
		static int					__sockNum;
		static t_events				__sockets;
		static t_Server				__servers;
		static t_Connections		__connections;
		std::vector<Server *>		__serverTemplates;


		void				readFile();
		void				firstCheck();
		void				checkBraces();
		void				reduceSpaces();
		void				setUpServers();
		void				setUpServer(size_t start);
		void				checkOuterscope(String outerScope);

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
		static bool			isServerSocket( int sd );

		void	initServers();
		void	mainLoop();
		ServerManager();

	public:
		ServerManager( const String &configutation_file );
		ServerManager( const ServerManager &copy );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();

		void	setUpWebserv();

		void	debug()
		{
			t_Server::iterator it = ServerManager::__servers.begin();
			for (; it != ServerManager::__servers.end(); it++) {
				(*it).second->print();
			}

		};
};

#endif
