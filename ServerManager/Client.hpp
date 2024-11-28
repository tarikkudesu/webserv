#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Location.hpp"

class Client
{
	private:
		t_method						__method;
		String							__URI;
		String							__protocole;
		String							__requestLine;
		String							__requestHeaders;
		std::map< String, String >		__headerFeilds;

	public:
		Client(void);
		Client( String requestLine, String requestHeaders );
		Client( const Client &copy );
		Client& operator=( const Client &assign );
		~Client();

		String	getHeaderFeildValue( const String &key );
		void	proccessHeaders();
		void	proccessRequestLine();
		void	parseRequest();
};

#endif