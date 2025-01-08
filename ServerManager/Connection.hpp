#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "PackageResponse/Response.hpp"

typedef std::map< int, Server* >			t_Server;
typedef std::vector< Server * >				t_serVect;

class Connection
{
	private :
		int						__sd;
		size_t					__erase;
		Request					__request;
		t_Server				*__serversP;

		String					identifyChunks( String &currBuff );
		String					identifyRequestHeaders();
		String					identifyRequestLine();
		void					identifyRequestBody();
		Server					*identifyServer();
		void					responseBuilder();
		void					requestParser();
		Connection();

	public:
		String					__buff;
		std::queue< String >	__responseQueue;
		bool					__readable;

		void					setServers( t_Server &servers );
		void					proccessData( String input );
		int						getSock();


		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();
};

#endif
