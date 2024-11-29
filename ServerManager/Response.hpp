#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include "Client.hpp"

class Response
{
	private :
		Client								__client;
		static std::map< int16_t, String >	__errCode;
		static String						__errPage;

	public:
		Response();
		Response( const Client &client );
		Response( const Response &copy );
		Response	&operator=( const Response &assign );
		~Response();

		static	String	buildErrorPage( int16_t errCode );
		static	void	replaceString( String &original, const String toReplace, const String replacement );
};

#endif
