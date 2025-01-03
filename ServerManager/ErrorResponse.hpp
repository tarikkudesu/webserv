#ifndef __ERRORRESPONSE_HPP__
# define __ERRORRESPONSE_HPP__

# include "Server.hpp"

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
		String								__reasonPhrase;
		static String						__errPage;

		static void							initErrorCodes();
		void								buildStatusLine();
		void								buildHeaderFeilds();
		void								buildResponseBody();
		void								constructErrorPage();
		ErrorResponse();

	public:
		static std::map< int16_t, String >	__errCode;
		String								getResponse() const;


		ErrorResponse( int code, String indication );
		ErrorResponse( int code, Location &location, String indication );
		ErrorResponse( const ErrorResponse &copy );
		ErrorResponse	&operator=( const ErrorResponse &assign );
		~ErrorResponse();
};

#endif
