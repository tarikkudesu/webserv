#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "../utilities/Token.hpp"

class Location
{
	private :
		bool								b__r; // temporary usage

		void								proccessClientBodyBufferSizeToken( t_svec &tokens );
		void								proccessAllowMethodsDirective( t_svec &tokens );
		void								proccessAuthenticateDirective(t_svec &tokens);
		void								proccessErrorPageDirective( t_svec &tokens );
		void								proccessAutoindexDirective( t_svec &tokens );
		void								proccessCgiPassDirective( t_svec &tokens );
		void								proccessReturnDirective( t_svec &tokens );
		void								proccessIndexDirective( t_svec &tokens );
		void								proccessRootDirective( t_svec &tokens );
		void								proccessToken(t_svec &tokens );
		void								LocationBlock( size_t pos );
		void								addDirective( size_t end );
		void								proccessDirectives();
		void								parseDirectives();
		void								parse();
		Location();

	public:
		const String						__path;
		String								__line; // temporary usage
		String								__root;
		t_svec								__index;
		String								__return;
		String								__cgiPass;
		bool								__autoindex;
		std::deque< String >				__directives;
		std::map< int16_t, String >			__errorPages;
		std::vector< t_method >				__allowMethods;
		String								__authenticate;
		long								__clientBodyBufferSize;


		Location( const Location &copy );
		Location( const String &conf, const String &root );
		Location( const String &dir, const String &conf, const String &root );
		Location	&operator=( const Location &assign );
		~Location();
};

String methodToString(t_method t);
std::ostream &operator<<( std::ostream &o, const Location &loc );

#endif
