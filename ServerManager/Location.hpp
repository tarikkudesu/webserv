#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "WSU.hpp"

class Location
{
	private :
		String							__line;
		String							__root;
		String							__index;
		bool							__dirListing;
		std::deque< String >			__directives;
		std::map< int16_t, String >		__errorPages;
		std::vector< t_method >			__allowMethods;
		std::map< String, Location* >	__subLocations;

		void							proccessToken(std::vector<String> &tokens);
		void							proccessDirectives();
		void							addLocationBlock(size_t pos);
		void							addDirective(size_t end);
		void							parseDirectives();
		void							addErrPages();

	public:
		Location();
		Location( String dir );
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();

		void		parseLocation( String conf );
};

#endif
