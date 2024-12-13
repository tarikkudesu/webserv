#ifndef TEMPLATE_HPP
# define TEMPLATE_HPP

# include "Location.hpp"

class Template
{
	private :
		std::deque< String >			__directives;
		String							__line;
		String							__host;
		String							__root;
		String							__index;
		std::vector< int16_t >			__ports;
		std::vector< String >			__serverNames;
		std::vector< t_method >			__allowMethods;
		std::map< int16_t, String >		__errorPages;
		size_t							__clientBodyBufferSize;
		bool							b__clientBodyBufferSize;
		bool							b__host;

		void	proccessHostToken( std::vector<String> &tokens );
		void	proccessListenToken( std::vector<String> &tokens );
		void	proccessServerNameToken( std::vector<String> &tokens );
		void	proccessClientBodyBufferSizeToken( std::vector<String> &tokens );

		void	proccessToken(std::vector<String> &tokens);
		void	proccessDirectives();
		void	parseDirectives();
		void	addDirective( size_t pos );
		void	addLocationBlock( size_t pos );
		void	parse();
		Template();

	public:
		Template( String &line );
		Template( const Template &copy );
		Template	&operator=( const Template &assign );
		~Template();

};

#endif
