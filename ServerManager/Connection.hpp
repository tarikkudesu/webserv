#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Server.hpp"

class Connection
{
	private :
		int						__sd;
		String					__buff;
		Client					__client;
		String					__requestBody;
		String					__headerFeilds;
		size_t					__erase;

		String		identifyRequestHeaders();
		String		identifyRequestLine();
		void		identifyRequestBody();
	public:

		std::queue< String >	__responseQueue;

		Connection();
		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();


		void		proccessInput( String input );
};

#endif
