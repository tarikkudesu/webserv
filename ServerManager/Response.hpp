#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include "Request.hpp"

class Response
{
	private :
		Request				&__request;
		Server				&__server;
		int					statusCode;;
		String				reasonPhrase;
		String				body;
		std::map<String,	String> heaaderResponse;

	public:

		void	build();


		Response( Request &request, Server &server );
		Response( const Response &copy );
		Response	&operator=( const Response &assign );
		~Response();
};

#endif
