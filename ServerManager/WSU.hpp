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
		static void								terr( char *__error_message );
		static void								terr( String __error_message );
		static void								file_error( String __error_message, String __file, String __line );


		static void								l1();
		static void								l1( String s );
		static void								l1( int i );

		static void								trimSpaces( String &str );
		static std::vector<std::string>			splitBySpaces( const std::string &input );
		static int 								stringToInt(const std::string& str);
		static std::string 						intToString(int number);
};

#endif
