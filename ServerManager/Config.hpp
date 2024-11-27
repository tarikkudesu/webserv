#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include "Connection.hpp"

class Config
{
	private :

	public:
		Config( String &configuration_file );
		Config( const Config &copy );
		Config	&operator=( const Config &assign );
		~Config();
};

#endif
