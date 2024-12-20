#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Connection.hpp"

typedef std::map< int, Connection* >		t_Connections;

void printLocalAddress(int sockfd);

class ServerManager
{
	private :
/************************************************************************
 *                            SERVER CONTROL                            *
 ************************************************************************/
		static int					__sockNum;
		static t_events				__sockets;
		static t_Server				__servers;
		static struct pollfd		*__events;
		static t_Connections		__connections;

		static void					addSocket( int sd, t_endian endian );
		static void					addServer( Server *server );
		static void					removeConnection( int sd );
		static void					addConnection( int sd );
		static void					removeSocket( int sd );
		static void					removeServer( int sd );
		static bool					isServerSocket( int sd );
		void						writeDataToSocket( int sd );
		void						readDataFromSocket( int sd );
		void						acceptNewConnection( int sd );
		void						proccessPollEvent( int sd, int &retV );
		void						mainLoop();

/*************************************************************************
 *                             SERVER PARSER                             *
 *************************************************************************/
		String						__lines;
		String						__config;
		t_serVect					__serverTemplates;

		void						readFile();
		void						checkHosts();
		void						firstCheck();
		void						checkBraces();
		void						reduceSpaces();
		void						setUpServers();
		void						setUpServer(size_t start);
		void						checkOuterscope(String outerScope);
		void						checkConflicts();
		void						initServers();
		void						logServers();

		ServerManager();

	public:
		void						setUpWebserv();
		void						debug()
		{
			// t_Server::iterator it = ServerManager::__servers.begin();
			// for (; it != ServerManager::__servers.end(); it++) {
			// 	(*it).second->print();
			// }
		};


		ServerManager( const ServerManager &copy );
		ServerManager( const String &configutation_file );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();
};

#endif
