#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include "ErrorResponse.hpp"

class Request
{
	private:
		void							clear();
		void							hostAndPort();
		void							contentLength();
		void							connectionType();
		void							transferEncoding();
		String							getHeaderFeildValue( const String &key );
		void							proccessHeaders( String requestHeaders );
		void							proccessRequestLine( const String &requestLine );

	public:
		String							__URI;
		String							__host;
		int								__port;
		t_method						__method;
		String							__protocole;
		std::map< String, String >		__headerFeilds;
		t_transferEncoding				__transferEncoding;
		t_connectionType				__connectionType;
		size_t							__contentLength;
		String							__requestbody;

		bool							hasBody();
		bool							connectionTypeClose();
		void							parseRequest( const String &requestLine, const String &requestHeaders );


		Request();
		Request( const Request &copy );
		Request& operator=( const Request &assign );
		~Request();
};

#endif
