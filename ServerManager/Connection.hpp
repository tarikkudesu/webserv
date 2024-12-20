#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Response.hpp"

typedef std::map< int, Server* >			t_Server;
typedef std::vector< Server * >				t_serVect;

class Connection
{
	private :
		int						__sd;
		String					__buff;
		size_t					__erase;
		Request					__request;
		t_Server				*__serversP;


		String					identifyChunks( String &currBuff );
		String					identifyRequestHeaders();
		String					identifyRequestLine();
		void					identifyRequestBody();
		Location				*identifyLocation();
		Server					*identifyServer();
		void					responseBuilder();
		void					requestParser();

		Connection();

	public:
		std::queue< String >	__responseQueue;

		void					setServers( t_Server &servers );
		void					proccessData( String input );


		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();
};

#endif
