#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include "Headers.hpp"

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
		size_t							__bodySize;
		String							__fragement;
		String							__protocole;
		String							__queryString;
		std::map< String, String >		__headerFeilds;
		std::vector< s_body >			__body;

		void							parseRequest(String requestLine, String requestHeaders);


		Request();
		Request( const Request &copy );
		Request& operator=( const Request &assign );
		~Request();
};

std::ostream &operator<<(std::ostream &o, const Request &req );

#endif
