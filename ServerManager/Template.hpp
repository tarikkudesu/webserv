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

		void	parseDirectives();
		void	addDirective( size_t pos );
		void	addLocationBlock( size_t pos );
	public:
		Template( String &line );
		Template( const Template &copy );
		Template	&operator=( const Template &assign );
		~Template();

		void							parse();
};

#endif
