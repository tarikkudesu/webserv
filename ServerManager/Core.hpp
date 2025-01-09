#ifndef __CORE_HPP__
# define __CORE_HPP__

#include "Connection.hpp"

typedef std::map< int, Connection* >		t_Connections;


/************************************************************************
 *                            SERVER CONTROL                            *
 ************************************************************************/

class Core
{
	private:
		static int					__sockNum;
		static t_events				__sockets;
		static t_Server				__servers;
		static struct pollfd		*__events;
		static t_Connections		__connections;


		static void					removeSocket( int sd );
		static void					writeDataToSocket( int sd );
		static void					readDataFromSocket( int sd );
		static void					acceptNewConnection( int sd );
		static void					addSocket( int sd, t_endian endian );
		static void					proccessPollEvent( int sd, int &retV );

		Core();
		Core( const Core &copy );
		Core	&operator=( const Core &assign );
		~Core();

	public:
		static void					logServers();
		static void					checkConflicts();
		static void					removeServer( int sd );
		static void					addConnection( int sd );
		static bool					isServerSocket( int sd );
		static void					removeConnection( int sd );
		static void					addServer( Server *server );

		static bool					up;
		static void 				clear();
		static void					mainLoop();

};

#endif
