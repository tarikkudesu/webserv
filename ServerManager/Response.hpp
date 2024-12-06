#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include "Request.hpp"

class Response : public ErrorResponse
{
	private :
		std::vector< String >				__Content;

		// void			buildStatusLine();
		// void			buildHeaderFeilds();
		// void			buildResponseBody();

	public:
		Response();
		Response( const Response &copy );
		Response	&operator=( const Response &assign );
		~Response();
};

#endif
