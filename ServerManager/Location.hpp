#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "WSU.hpp"

class Location
{
	private :
		bool								b__r;

		String								__line;
		String								__root;
		t_svec								__index;
		String								__return;
		String								__cgiPass;
		bool								__autoindex;
		std::deque< String >				__directives;
		std::map< int16_t, String >			__errorPages;
		std::vector< t_method >				__allowMethods;
		static std::map< int16_t, String >	__defaultErrorPages;

		void								proccessAllowMethodsDirective( t_svec &tokens );
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
		void								addErrPages();
		void								parse();
		Location();

	public:
		const String						__path;

		Location( const String &conf );
		Location( const String &dir, const String &conf );
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();
};

#endif
