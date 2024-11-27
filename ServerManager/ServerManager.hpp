#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Config.hpp"

typedef std::vector< pollfd >		t_events;
typedef std::map< int, Server >		t_Server;
typedef std::map< int, Connection >	t_Connections;

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
		void	addServer( Server &server );
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
};

#endif
