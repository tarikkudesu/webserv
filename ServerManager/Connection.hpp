#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Response.hpp"

typedef std::map< int, Server* >			t_Server;
typedef std::map< int, Template* >			t_Template;

class Connection
{
	private :
	public:
		int						__sd;
		String					__buff;
		size_t					__erase;
		Request					__request;
		t_Server				*__serversP;


		void		setServers( t_Server &servers );
		String		identifyRequestHeaders();
		String		identifyRequestLine();
		void		identifyRequestBody();
		Server		&identifyServer();
		String		identifyChunks( String &currBuff );

	// public:
		std::queue< String >	__responseQueue;

		Connection();
		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();

		void		requestParser();
		void		responseBuilder();
		void		proccessData( String input );
};

#endif
