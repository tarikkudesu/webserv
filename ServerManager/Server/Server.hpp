#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Config/Config.hpp"

class Server
{
	public:

		int		__sd;
		int		__port;
		String	__serverName;

		Server();
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();

		static void		setNonBlockingMode( int sd );
		void			setup();
};

#endif
