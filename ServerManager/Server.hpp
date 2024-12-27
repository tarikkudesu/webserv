#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Location.hpp"

class Server
{
	private:
		int								__sd;
		int								__port;
		String							__host;
		std::vector< int >				__ports;
		std::vector< Location >			__locations;
		t_svec							__directives;
		t_svec							__serverNames;
		long							__clientBodyBufferSize;

		void							proccessServerDirectives();
		void							proccessToken(t_svec &tokens);
		void							proccessHostToken( t_svec &tokens );
		void							parseServerDirectives( String line );
		void							proccessListenToken( t_svec &tokens );
		void							addDirective( String &line, size_t end );
		void							proccessServerNameToken( t_svec &tokens );
		void							LocationBlock( String &line, size_t pos );
		void							parseLocation( String line, String parent );
		void							proccessClientBodyBufferSizeToken( t_svec &tokens );
		void							addLocation( String &line, size_t pos, String parent );
		void							proccessLocation(String &location, size_t pos, String &parent);
		Server();

	public:
		bool							__valid;

		bool							amITheServerYouAreLookingFor( const String &sN );
		Location						&identifyLocation( const String &URI );
		const t_svec					&getServerNames() const;
		const String					&getServerHost() const;
		int								getServerSocket() const;
		int								getServerPort() const;
		void							setPort(int port);
		String							serverIdentity();
		std::vector< int >				&getPorts();
		void							setup();


		Server( String line );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();
};

#endif
