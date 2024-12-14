#ifndef TEMPLATE_HPP
# define TEMPLATE_HPP

# include "Location.hpp"

class Template
{
	private :
		std::deque< String >			__locations;
		std::deque< String >			__directives;
		Location						__rootLocation;
		String							__line;
		String							__host;
		std::vector< int16_t >			__ports;
		std::vector< String >			__serverNames;
		size_t							__clientBodyBufferSize;
		bool							b__clientBodyBufferSize;
		bool							b__host;

		void	proccessHostToken( std::vector<String> &tokens );
		void	proccessListenToken( std::vector<String> &tokens );
		void	proccessServerNameToken( std::vector<String> &tokens );
		void	proccessClientBodyBufferSizeToken( std::vector<String> &tokens );

		void	proccessToken(std::vector<String> &tokens);
		void	proccessDirectives();
		void	proccessLocations();
		void	parseDirectives();
		void	addDirective( size_t pos );
		void	LocationBlock( size_t pos );
		void	parse();
		Template();

	public:
		Template( String &line );
		Template( const Template &copy );
		Template	&operator=( const Template &assign );
		~Template();
};

#endif
