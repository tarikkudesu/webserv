#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Location.hpp"

class Server
{
	private:
		int								__sd;
		int								__port;
		String							__host;
		t_svec							__directives; // temporary usage

		void							proccessServerDirectives();
		void							proccessToken(t_svec &tokens);
		void							proccessHostToken( t_svec &tokens );
		void							parseServerDirectives( String line );
		void							proccessListenToken( t_svec &tokens );
		void							addDirective( String &line, size_t end );
		void							proccessServerNameToken( t_svec &tokens );
		void							LocationBlock( String &line, size_t pos );
		void							parseLocation( String line, String parent, String root );
		void							addLocation( String &line, size_t pos, String parent );
		void							proccessLocation(String &location, size_t pos, String &parent);
		Server();

	public:
		bool							__valid; // temporary usage
		std::vector< int >				__ports; // temporary usage
		std::vector< Location >			__locations;
		t_svec							__serverNames;

		bool							amITheServerYouAreLookingFor( const String &sN );
		Location						&identifyLocation( const String &URI );
		int								getServerSocket() const;
		String							serverIdentity() const;
		const String					&getServerHost() const;
		int								getServerPort() const;
		void							setPort(int port);
		void							setup();


		Server( String line );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();
};

std::ostream &operator<<( std::ostream &o, const Server &ser );

#endif
