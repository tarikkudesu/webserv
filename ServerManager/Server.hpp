#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "Location.hpp"

class Server
{
	private:
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

		void							proccessClientBodyBufferSizeToken( t_strVect &tokens );
		void							proccessServerNameToken( t_strVect &tokens );
		void							proccessListenToken( t_strVect &tokens );
		void							proccessHostToken( t_strVect &tokens );
		void							proccessToken(t_strVect &tokens);
		void							proccessDirectives();
		void							LocationBlock( size_t pos );
		void							addDirective( size_t pos );
		void							parseDirectives();
		void							parse();
		Server();

	public:
		bool							__valid;

		bool							amITheServerYouAreLookingFor( const String &sN);
		const t_strVect					&getServerNames() const;
		const String					&getServerHost() const;
		int								getServerSocket() const;
		int								getServerPort() const;
		void							setPort(int port);
		String							serverIdentity();
		std::vector< int >				&getPorts();
		void							setup();

		void							print()
		{
			std::cout << "Server\n";
			std::cout << "\thost: " << __host << "\n";
			std::cout << "\tserver_name: ";
			for (t_strVect::iterator it = __serverNames.begin(); it != __serverNames.end(); it++) {
				std::cout << *it << " ";
			} std::cout << "\n";
			std::cout << "\tclientBodyBufferSize: " << __clientBodyBufferSize << "\n";
			std::cout << "\tport: " << __port << "\n";
			this->__rootLocation.print();
		}


		Server( String &line );
		Server( const Server &copy );
		Server	&operator=( const Server &assign );
		~Server();
};

#endif
