#ifndef __ERRORRESPONSE_HPP__
# define __ERRORRESPONSE_HPP__

# include "Server.hpp"

class ErrorResponse
{
	private :
		int16_t								__code;
		String								__reasonPhrase;
		String								__errorIndication;
		String								__finalResponse;
		String								__StatusLine;
		String								__headers;
		String								__Body;

		void								buildStatusLine();
		void								buildHeaderFeilds();
		void								buildResponseBody();

		ErrorResponse();

	protected :
		static std::map< int16_t, String >	__errCode;
		static String						__errPage;

	public:
		const String		&getResponse();
		static	void		initErrorCodes();
		void				constructErrorPage();


		ErrorResponse( int code, String error );
		ErrorResponse( const ErrorResponse &copy );
		ErrorResponse	&operator=( const ErrorResponse &assign );
		~ErrorResponse();
};

#endif
