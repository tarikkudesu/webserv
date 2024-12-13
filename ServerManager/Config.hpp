#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include "Connection.hpp"

class Config
{
	private :
		t_Server				__servers;
		t_Template				__templates;
		String					__configFile;
		String					__lines;
		std::fstream			__fS;

		void					readFile();
		void					firstCheck();
		void					checkBraces();
		void					reduceSpaces();
		void					setUpServers();
		void					setUpServer(size_t start);
		void					checkOuterscope(String outerScope );

	public:
		Config( String configuration_file );
		Config( const Config &copy );
		Config	&operator=( const Config &assign );
		~Config();

		void					setupEverything();
};

#endif
