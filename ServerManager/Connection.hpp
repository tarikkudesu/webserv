#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Response.hpp"

typedef std::map< int, Server* >			t_Server;
typedef std::vector< Server * >				t_serVect;

typedef struct s_storage
{
	String fileName;
	std::ofstream fs;
} t_storage;


class Connection
{
	private :
		int						__sd;
		size_t					__erase;
		Request					__request;
		t_Server				*__serversP;
		t_storage				__storage;

		String					identifyRequestHeaders();
		String					identifyRequestLine();
		Server					*identifyServer();
		void					indentifyRequestBody();
		void					processResponse();
		void					processRequest();
		void					processCunkedBody();
		void					processDefinedBody();
		void					processMultiPartBody();
		Connection();

	public:
		String					__buff;
		std::queue< String >	__responseQueue;

		void					setServers( t_Server &servers );
		void					proccessData( String input );
		int						getSock();


		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();
};

#endif
