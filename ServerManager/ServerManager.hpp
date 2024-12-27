#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Connection.hpp"

typedef std::map< int, Connection* >		t_Connections;

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

		void						proccessPollEvent( int sd, int &retV );
		static void					addSocket( int sd, t_endian endian );
		void						acceptNewConnection( int sd );
		void						readDataFromSocket( int sd );
		void						writeDataToSocket( int sd );
		static void					addServer( Server *server );
		static void					removeConnection( int sd );
		static bool					isServerSocket( int sd );
		static void					addConnection( int sd );
		static void					removeSocket( int sd );
		static void					removeServer( int sd );
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
		void						logServers();
		void						checkBraces();
		void						initServers();
		void						reduceSpaces();
		void						setUpServers();
		void						checkConflicts();
		void						setUpServer(size_t start);
		void						checkOuterscope(String outerScope);
		ServerManager();

	public:
		static bool					up;

		void						setUpWebserv();
		static void					clear();


		ServerManager( const ServerManager &copy );
		ServerManager( const String &configutation_file );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();
};

#endif
