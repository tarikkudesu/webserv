#ifndef CSTRING_HPP
# define CSTRING_HPP

#include "webserv.hpp"

class cString
{
	private :
		char					*__buff;
		size_t					__size;

	public:

		cString					substr( size_t start, size_t length );
		void					erase( size_t start, size_t end );
		size_t					find( const String &f ) const;
		void					join( const cString &j );
		std::string 			to_string() const;
		char					*getBuff() const;
		size_t					length() const;
		cString					duplicate() const;
		void					clear();



		cString();
		cString( char *buff, size_t size );
		cString( const cString &copy );
		cString	&operator=( const cString &assign );
		~cString();
};

std::ostream& operator<<(std::ostream& out, const cString& str);

#endif
