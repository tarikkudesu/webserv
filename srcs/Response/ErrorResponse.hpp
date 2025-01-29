#ifndef __ERRORRESPONSE_HPP__
# define __ERRORRESPONSE_HPP__

# include "../ServerManager/Server.hpp"

class ErrorResponse
{
	private :
		String								__page;
		int16_t								__code;
		String								__Body;
		String								__headers;
		Location							*__location;
		String								__StatusLine;
		String 								__indication;
		String 								__redirection;
		String								__reasonPhrase;
		static String						__errPage;

		void								buildStatusLine();
		void								buildHeaderFeilds();
		void								buildResponseBody();
		void								constructErrorPage();
		ErrorResponse();

	public:
		BasicString							getResponse() const;
		void								print() const;

		ErrorResponse( int code, String indication );
		ErrorResponse( int code, Location &location, String indication );
		ErrorResponse( int code, String redirection , Location &location);
		ErrorResponse( const ErrorResponse &copy );
		ErrorResponse	&operator=( const ErrorResponse &assign );
		~ErrorResponse();
};

std::ostream &operator<<(std::ostream &o, const ErrorResponse &r);

#endif
