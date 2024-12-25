#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "WSU.hpp"

class Location
{
	private :
		bool								b__r;
		bool								b__root;
		bool								b__index;
		bool								b__autoindex;
		bool								b__allowMethods;


		String								__line;
		String								__root;
		t_svec								__index;
		String								__return;
		String								__cgiPass;
		bool								__autoindex;
		std::deque< String >				__directives;
		std::map< int16_t, String >			__errorPages;
		std::map< String, Location* >		__subLocations;
		std::vector< t_method >				__allowMethods;
		static std::map< int16_t, String >	__defaultErrorPages;

		void								allowMethodsDirective( t_svec &tokens );
		void								errorPageDirective( t_svec &tokens );
		void								autoindexDirective( t_svec &tokens );
		void 								checkNestedLocation( String &path );
		void								cgiPassDirective( t_svec &tokens );
		void								returnDirective( t_svec &tokens );
		void								indexDirective( t_svec &tokens );
		void								rootDirective( t_svec &tokens );
		void								proccessToken(t_svec &tokens );
		void								addLocationBlock( size_t pos );
		void								addDirective( size_t end );
		void								proccessDirectives();
		void								parseDirectives();
		void								addErrPages();

		Location( const String &dir );

	public:
		const String						__path;

		void								locationMatch( const String &path, Location *&location );
		void								parseLocation( String conf );
		void								print();

		Location();
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();
};

#endif
