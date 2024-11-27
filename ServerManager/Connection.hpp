#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include "Server.hpp"

class Connection
{
	private :
		int						__sd;
		Client					__client;
		String					__requestBody;
		String					__headerFeilds;
		t_connectionType		__connectionType;
		t_requestPhase			__requestPhase;

	public:
		std::queue< String >	__responseQueue;
		Connection();
		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();

		String		identifyRequestLine( String::iterator &start, String::iterator &end );
		String		identifyHeaders( String::iterator &start, String::iterator &end );
		void		proccessInput( String input );
};

#endif
