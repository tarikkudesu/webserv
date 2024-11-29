#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Response.hpp"

class Server
{

	public:
		int								__sd;
		int								__port;
		String							__serverIP;
		String							__serverName;
		bool							__serverDown;
		Location						__rootLocation;

		Server();
		Server( String serverName, int port );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();

		String			getServerIP() const;
		int				getServerPort() const;
		bool			getServerStat() const;
		String			getServerName() const;
		int				getServerSocket() const;


		void			stopServer();
		void			setup();

		static void		setNonBlockingMode( int sd );
};

#endif
