#ifndef __WSU_HPP__
# define __WSU_HPP__

# include "../srcs/webserv.hpp"

class wsu
{
	private :
		wsu();
		wsu( const wsu &copy );
		wsu	&operator=( const wsu &assign );
		~wsu();

	public:
		static bool								__criticalOverLoad;

		static String							logDate();
		static String							buildIMFDate();
		static void								log( String __log_message );
		static void								warn( String __log_message );
		static void								error( String __log_message );
		static void								terr( char *__error_message );
		static void								terr( String __error_message );
		static void								debug( String __log_message );
		static void								running( String __log_message );
		static void								success( String __log_message );

		static String 							intToString(int number);
		static struct pollfd					*data( t_events &events );
		static void								trimSpaces( String &str );
		static int 								hexToInt(const String& str);
		static void								setNonBlockingMode( int sd );
		static int 								stringToInt(const String& str);
		static void								toLowerString( String &input );
		static void								toUpperString( String &input );
		static std::vector<String>				splitBySpaces( const String &input );
		static std::vector<String>				splitByChar( const String &input, char del );
		static bool								samePath(const String &path1, const String &path2 );
		static bool								containsPath(const String &path, const String &subPath );
		static String							mergeByChar( const std::vector<String> &input, char del );
		static void								replaceString(String &original, const String toReplace, const String replacement);
};

#endif
