#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include "Connection.hpp"

class Config
{
	private :
		t_Server				*__serversP;
		String					__configFile;
		String					__lines;
		std::fstream			__fS;

		void		readFile();
		void		firstCheck();
		void		reduceSpaces();
		void		checkBraces();
		void		setUpServer(size_t start);
		void		setUpServers();
		void		checkOuterscope(String outerScope );
		static void	trimSpaces( String &str );

	public:
		Config( String configuration_file );
		Config( const Config &copy );
		Config	&operator=( const Config &assign );
		~Config();

		void	setServers( t_Server &servers );
		void	setupEverything();
};

#endif
