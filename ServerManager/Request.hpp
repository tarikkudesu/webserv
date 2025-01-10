#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include "Headers.hpp"


struct	s_body {
	t_svec				_headers;
	String				_fileName;
};


class Request
{
	private:
		void							clear();
		void							proccessURI();
		void							proccessHeaders( String requestHeaders );
		void							proccessRequestLine( const String &requestLine );

	public:
		String							__URI;
		e_requestPhase					__phase;
		t_method						__method;
		Headers							__headers;
		String							__fragement;
		String							__protocole;
		String							__requestbody;
		std::map< String, String >		__headerFeilds;
		std::map< String, String >		__queryVariables;
		std::vector< s_body >			__body;

		void							parseRequest( const String &requestLine, const String &requestHeaders );


		Request();
		Request( const Request &copy );
		Request& operator=( const Request &assign );
		~Request();
};

std::ostream &operator<<(std::ostream &o, const Request &req );

#endif
