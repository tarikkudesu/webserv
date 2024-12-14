#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Template.hpp"

class Server
{
	public:
		int								__sd;
		int16_t							__port;
		String							__serverName;
		bool							__serverDown;
		Location						__DefaultLocation;

		Server();
		Server( String config );
		Server( String serverName, int port );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();

		int16_t			getServerPort() const;
		bool			getServerStat() const;
		String			getServerName() const;
		int				getServerSocket() const;


		void			stopServer();
		void			setup();

		static void		setNonBlockingMode( int sd );
};

#endif
