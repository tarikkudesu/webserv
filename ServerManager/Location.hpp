#ifndef LOCATION_HPP
# define LOCATION_HPP

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
