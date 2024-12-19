#ifndef __WSU_HPP__
# define __WSU_HPP__

# include "../srcs/webserv.hpp"

class WSU
{
	private :
		WSU();
		WSU( const WSU &copy );
		WSU	&operator=( const WSU &assign );
		~WSU();

	public:
		static bool								__criticalOverLoad;

		static std::string						logDate();
		static std::string						buildIMFDate();
		static void								log( String __log_message );
		static void								warn( String __log_message );
		static void								error( String __log_message );
		static void								terr( char *__error_message );
		static void								terr( String __error_message );
		static void								debug( String __log_message );
		static void								running( String __log_message );
		static void								success( String __log_message );

		static void								trimSpaces( String &str );
		static void								setNonBlockingMode( int sd );
		static int 								hexToInt(const std::string& str);
		static int 								stringToInt(const std::string& str);
		static void								toLowerString( std::string &input );
		static void								toUpperString( std::string &input );
		static void								replaceString(String &original, const String toReplace, const String replacement);
		static std::vector<std::string>			splitBySpaces( const std::string &input );
		static std::vector<std::string>			splitByChar( const std::string &input, char del );
		static std::string 						intToString(int number);
};

#endif
