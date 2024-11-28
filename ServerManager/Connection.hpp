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
		t_connectionType		__connectionType;
		t_requestPhase			__requestPhase;
		size_t					__erase;

	public:
		std::queue< String >	__responseQueue;
		Connection();
		Connection( int sd );
		Connection( const Connection &copy );
		Connection	&operator=( const Connection &assign );
		~Connection();

		void		identifyTransferEncoding();
		void		identifyConnectionType();
		String		identifyHeaders();
		String		identifyRequestLine();
		void		proccessInput( String input );
};

#endif
