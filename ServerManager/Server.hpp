#ifndef SERVER_HPP
# define SERVER_HPP

# include "Response.hpp"

class Server
{

	public:
		int		__sd;
		int		__port;
		String	__serverName;
		bool	__serverDown;
		/*********************
		 * TEMPORARLY PUBLIC *
		 *********************/

		Server();
		Server( String serverName, int port );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();

		int		getServerPort() const;
		bool	getServerStat() const;
		String	getServerName() const;
		int		getServerSocket() const;

		void			stopServer();
		void			setup();

		static void		setNonBlockingMode( int sd );
};

#endif
