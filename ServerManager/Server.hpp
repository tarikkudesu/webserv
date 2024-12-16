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

		bool							__valid;
		int								getServerSocket() const;
		int								getServerPort() const;
		void							setPort(int port);
		std::vector< int >				&getPorts();
		void							setup();
		void							clean()
		{
			__ports.clear();
			__directives.clear();
		}
		void							print() {
			std::cout << "Server\n";
			std::cout << "\thost: " << __host << "\n";
			std::cout << "\tserver_name: ";
			for (std::vector<String>::iterator it = __serverNames.begin(); it != __serverNames.end(); it++) {
				std::cout << *it << " ";
			} std::cout << "\n";
			std::cout << "\tport: " << __port << "\n";
			this->__rootLocation.print();
			// for (std::vector<int>::iterator it = __ports.begin(); it != __ports.end(); it++) {
			// 	std::cout << *it << " ";
			// } std::cout << "\n";
		}
};

#endif
