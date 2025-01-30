#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Location.hpp"

class Server
{
	private:
		int								__sd;
		int								__port;
		String							__host;
		String							__root;
		t_svec							__directives; // temporary usage

		void							proccessServerDirectives();
		void							proccessToken(t_svec &tokens);
		void							proccessRootToken( t_svec &tokens );
		void							proccessHostToken( t_svec &tokens );
		void							parseServerDirectives( String line );
		void							proccessListenToken( t_svec &tokens );
		void							proccessServerNameToken( t_svec &tokens );
		void							parseLocations( String line, String parent );
		void							skipLocationBlock( String &line, size_t pos );
		void							addServerDirective( String &line, size_t end );
		void							addLocationBlock( String &line, size_t pos, String parent );
		void							proccessLocationBlock(String &location, size_t pos, String &parent );
		Server();

	public:
		bool							__valid; // temporary usage
		std::vector< int >				__ports; // temporary usage
		std::map<String, String>		__tokenDB;
		std::vector< Location >			__locations;
		t_svec							__serverNames;

		bool							amITheServerYouAreLookingFor( const String &sN );
		Location						&identifyLocation( const String &URI );
		int								getServerSocket() const;
		String							serverIdentity();
		const String					&getServerHost() const;
		int								getServerPort() const;
		void							setPort(int port);
		void							LoadUsers();
		void							setup();


		Server( String line );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();
};

std::ostream &operator<<( std::ostream &o, Server &ser );

#endif
