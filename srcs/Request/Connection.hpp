#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "../Response/Response.hpp"

typedef std::map< int, Server* >			t_Server;
typedef std::vector< Server * >				t_serVect;

class Connection
{
	private :
		int								__sd;
		BasicString						__data;
		Request							__request;
		t_Server						*__serversP;
		static std::ofstream			__fs;

		Server							*identifyServer();
		void							processRequest();
		void							processResponse();
		void							processCunkedBody();
		void							initializeTmpFiles();
		void							processDefinedBody();
		void							indentifyRequestBody();
		void							processMultiPartBody();
		void							mpBody( t_multipartsection &part );
		void							mpHeaders( t_multipartsection &part );
		Connection();

	public:
		std::queue< BasicString >		__responseQueue;

		void							setServers( t_Server &servers );
		void							proccessData(BasicString);
		int								getSock();


		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();
};

#endif
