#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Location.hpp"

class Server
{
		int								__sd;
		int								__port;
		String							__line;
		String							__host;
		std::vector< int >				__ports;
		std::vector< String >			__directives;
		std::vector< String >			__serverNames;
		Location						__rootLocation;
		size_t							__clientBodyBufferSize;
		bool							b__clientBodyBufferSize;
		bool							b__host;

		void							proccessClientBodyBufferSizeToken( std::vector<String> &tokens );
		void							proccessServerNameToken( std::vector<String> &tokens );
		void							proccessListenToken( std::vector<String> &tokens );
		void							proccessHostToken( std::vector<String> &tokens );
		void							proccessToken(std::vector<String> &tokens);
		void							proccessDirectives();
		void							LocationBlock( size_t pos );
		void							addDirective( size_t pos );
		void							parseDirectives();
		void							parse();
		Server();

	public:
		Server( String &line );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();

		int								getServerSocket() const;
		int								getServerPort() const;
		void							setup();
};

#endif
