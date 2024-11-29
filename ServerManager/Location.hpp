#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "Logs.hpp"

class Location
{
	private :
		bool							__dirListing;
		bool							__rooted;
		String							__directory;
		String							__rootedDir;
		String							__defaultFile;
		std::map< String, Location >	__subLocations;
		std::vector< t_method >			__acceptedMethods;

	public:
		Location();
		Location( String dir );
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();
};

#endif
