#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "Logs.hpp"

class Location
{
	private :

	public:
		Location();
		Location( const Location &copy );
		Location	&operator=( const Location &assign );
		~Location();
};

#endif
