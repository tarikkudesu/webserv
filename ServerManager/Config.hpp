#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Server.hpp"

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
