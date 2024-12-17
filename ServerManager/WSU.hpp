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
		static std::string						logDate();
		static std::string						buildIMFDate();
		static void								accessLog( String info );
		static void								tout( String __log_message );
		static void								log( String __log_message );
		static void								warn( String __log_message );
		static void								error( String __log_message );
		static void								terr( char *__error_message );
		static void								terr( String __error_message );
		static void								file_error( String __error_message, String __file, String __line );

		static void								trimSpaces( String &str );
		static std::vector<std::string>			splitBySpaces( const std::string &input );
		static int 								stringToInt(const std::string& str);
		static std::string 						intToString(int number);
		static int 								hexToInt(const std::string& str);
		static void								replaceString(String &original, const String toReplace, const String replacement);
		static void								setNonBlockingMode( int sd );

};

#endif
