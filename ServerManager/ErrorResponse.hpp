#ifndef ERRORRESPONSE_HPP
# define ERRORRESPONSE_HPP

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

	protected :
		static std::map< int16_t, String >	__errCode;
		static String						__errPage;

	public:
		ErrorResponse();
		ErrorResponse( int code, String error );
		ErrorResponse( const ErrorResponse &copy );
		ErrorResponse	&operator=( const ErrorResponse &assign );
		~ErrorResponse();

		void				constructErrorPage();
		const String		&getResponse();
		static	void		initErrorCodes();
		static	void		replaceString( String &original, const String toReplace, const String replacement );
};

#endif
