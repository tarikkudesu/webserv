#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include "ErrorResponse.hpp"

class Request
{
	private:
		String							__URI;
		int16_t							__port;
		String							__host;
		String							__hostIP;
		t_method						__method;
		String							__protocole;
		std::map< String, String >		__headerFeilds;
		t_transferEncoding				__transferEncoding;
		t_connectionType				__connectionType;
		size_t							__contentLength;
		String							__requestbody;

		void					clear();
		void					hostAndPort();
		void					contentLength();
		void					connectionType();
		void					transferEncoding();

		String					getHeaderFeildValue( const String &key );
		void					proccessHeaders( String requestHeaders );
		void					proccessRequestLine( const String &requestLine );

	public:
		Request(void);
		Request( const Request &copy );
		Request& operator=( const Request &assign );
		~Request();


		bool					hasBody();
		void					setBody( const String &body );
		size_t					getContentLength();
		t_connectionType		getconnectionType();
		t_transferEncoding		gettransferEncoding();
		bool					connectionTypeClose();
		void					parseRequest( const String &requestLine, const String &requestHeaders );
};

#endif
