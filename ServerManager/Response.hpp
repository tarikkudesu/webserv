#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Client.hpp"

class Response
{
	private :
		static std::map< int16_t, String >	__errCode;
		static String						__errPage;

	public:
		Response();
		Response( const Response &copy );
		Response	&operator=( const Response &assign );
		~Response();

		static	String	buildErrorPage( int16_t errCode );
		static	void	replaceString( String &original, const String toReplace, const String replacement );
};

#endif
