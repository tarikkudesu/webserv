#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Server.hpp"

class Connection
{
	private :
	public:
		int						__sd;
		String					__buff;
		size_t					__erase;
		Request					__request;

		String		identifyRequestHeaders();
		String		identifyRequestLine();
		void		identifyRequestBody();
		String		identifyChunks( String &currBuff );

	// public:
		std::queue< String >	__responseQueue;

		Connection();
		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();

		void		requestParser();
		void		proccessData( String input );
};

#endif
