#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include "Request.hpp"

class Response
{
	private :
		Server				&__server;
		Request				&__request;
		Location			&__location;

	public:

		void	build();

		Response( Request &request, Server &server, Location &location );
		Response( const Response &copy );
		Response	&operator=( const Response &assign );
		~Response();
};

#endif
