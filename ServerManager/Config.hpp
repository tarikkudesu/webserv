#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include "Connection.hpp"

class Config
{
	private :
		std::vector<Server *>	__servers;
		String					__configFile;
		String					__lines;
		std::fstream			__fS;

		void					firstCheck();
		void					checkBraces();
		void					reduceSpaces();
		void					setUpServers();
		void					setupEverything();
		void					setUpServer(size_t start);
		void					checkOuterscope(String outerScope );

	public:
		Config( String configuration_file );
		Config( const Config &copy );
		Config	&operator=( const Config &assign );
		~Config();

		std::vector<Server *>	&getServers( void );
};

#endif
