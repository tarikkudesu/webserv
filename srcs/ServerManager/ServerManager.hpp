#ifndef __SERVERMANAGER_HPP__
# define __SERVERMANAGER_HPP__

# include "Core.hpp"

/*************************************************************************
 *                             SERVER PARSER                             *
 *************************************************************************/

class ServerManager
{
	private :
		String						__lines;
		String						__config;
		t_serVect					__serverTemplates;

		void						readFile();
		void						checkHosts();
		void						firstCheck();
		void						checkBraces();
		void						initServers();
		void						reduceSpaces();
		void						setUpServers();
		void						setUpServer(size_t start);
		void						checkOuterscope(String outerScope);
		ServerManager();

	public:
		void						setUpWebserv();

		ServerManager( const ServerManager &copy );
		ServerManager( const String &configutation_file );
		ServerManager	&operator=( const ServerManager &assign );
		~ServerManager();
};

#endif
