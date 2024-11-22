#ifndef __SERVER_ENGINE_HPP__
# define __SERVER_ENGINE_HPP__

# include "Server/Server.hpp"

typedef std::vector< pollfd >	t_events;
typedef std::vector< Server >	Servers;

class	ServerEngine
{
	private :
		String						__configurationFile;
		t_events					__sockets;
		Servers						__servers;
		int							__sockNum;

		ServerEngine( );

		void			addServerSocket( int sd );
		void			addClientSocket( int sd );
		void			removeSocket( int sd );
		bool			isServerSock( int sd );

		void			readDataFromSocket( struct pollfd &sockStruct );
		void			writeDataToSocket( struct pollfd &sockStruct );
		void			addNewConnection( struct pollfd &sockStruct );
		void			closeConnection( struct pollfd &sockStruct );

		void			proccessPollEvent( );
	public :
		ServerEngine( const String &configutation_file );
		ServerEngine( const ServerEngine& engine );
		ServerEngine& operator=( const ServerEngine& engine );
		~ServerEngine( );

		void			setUpWebserv( );
		void			mainLoop( );
};

#endif
