#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Location.hpp"

class Client
{
	private:
		String							__URI;
		t_method						__method;
		String							__protocole;
		std::map< String, String >		__headerFeilds;
		t_transferEncoding				__transferEncoding;
		t_connectionType				__connectionType;
		size_t							__contentLength;
		String							__requestbody;

		void					clear();
		void					contentLength();
		void					connectionType();
		void					transferEncoding();

		String					getHeaderFeildValue( const String &key );
		void					proccessHeaders( String requestHeaders );
		void					proccessRequestLine( const String &requestLine );

	public:
		Client(void);
		Client( const Client &copy );
		Client& operator=( const Client &assign );
		~Client();


		bool					hasBody();
		void					setBody( const String &body );
		size_t					getContentLength();
		t_connectionType		getconnectionType();
		t_transferEncoding		gettransferEncoding();
		bool					connectionTypeClose();
		void					parseRequest( const String &requestLine, const String &requestHeaders );
};

#endif
