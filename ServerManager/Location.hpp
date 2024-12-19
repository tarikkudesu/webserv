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
		t_strVect							__index;
		bool								__autoindex;
		std::deque< String >				__directives;
		std::map< int16_t, String >			__errorPages;
		std::map< String, Location* >		__subLocations;
		std::vector< t_method >				__allowMethods;
		t_strVect							__rootPath;

		void								allowMethodsDirective( t_strVect &tokens );
		void								errorPageDirective( t_strVect &tokens );
		void								autoindexDirective( t_strVect &tokens );
		void								indexDirective( t_strVect &tokens );
		void								rootDirective( t_strVect &tokens );
		void								proccessToken(t_strVect &tokens);
		void								addLocationBlock(size_t pos);
		void								addDirective(size_t end);
		void								proccessDirectives();
		void								parseDirectives();
		void								addErrPages();
		void								rootPath();

		Location( String dir );

	public:

		void		parseLocation( String conf );

		void		print() {
			std::cout << "root: " << this->__root << "\n";
			std::cout << "index: ";
			for (t_strVect::iterator it = __index.begin(); it != __index.end(); it++) {
				std::cout << *it << " ";
			} std::cout << "\n";
			if (__autoindex == true)
				std::cout << "autoindex: on\n";
			else
				std::cout << "autoindex: off\n";
			std::cout << "error_pages:\n";
			for (std::map< int16_t, String >::iterator it = __errorPages.begin(); it != __errorPages.end(); it++) {
				std::cout << "\t" << it->first << " " << it->second << "\n";
			} std::cout << "\n";
			std::cout << "allowed_methods:\n";
			for (std::vector< t_method >::iterator it = __allowMethods.begin(); it != __allowMethods.end(); it++) {
				if (*it == GET)
					std::cout << "\tGET";
				if (*it == DELETE)
					std::cout << "\tDELETE";
				if (*it == POST)
					std::cout << "\tPOST";
				if (*it == PUT)
					std::cout << "\tPUT";
				if (*it == OPTIONS)
					std::cout << "\tOPTIONS";
				if (*it == TRACE)
					std::cout << "\tTRACE";
				if (*it == CONNECT)
					std::cout << "\tCONNECT";
			} std::cout << "\n";
			for (std::map< String, Location* >::iterator it = __subLocations.begin(); it != __subLocations.end(); it++)
				it->second->print();
		}

		Location();
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();
};

#endif
