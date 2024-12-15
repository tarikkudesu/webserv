#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Config.hpp"

typedef std::map< int, Connection* >		t_Connections;
typedef std::vector<Location *>				t_Locations;

void printLocalAddress(int sockfd);

class ServerManager
{
	private :
		String						__config;
		static int					__sockNum;
		static t_events				__sockets;
		static t_Server				__servers;
		static t_Connections		__connections;

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

		ServerManager();
		void	mainLoop();

	public:
		ServerManager( const String &configutation_file );
		ServerManager( const ServerManager &copy );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();

		void	setUpWebserv();

		void	debug()
		{

		};
};

#endif
