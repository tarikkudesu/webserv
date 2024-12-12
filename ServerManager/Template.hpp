#ifndef TEMPLATE_HPP
# define TEMPLATE_HPP

# include "Location.hpp"

class Template
{
	protected :
		String							__line;
		String							__host;
		String							__root;
		String							__index;
		std::vector< int16_t >			__ports;
		std::vector< String >			__serverNames;
		std::vector< t_method >			__allowMethods;
		std::map< int16_t, String >		__errorPages;
		size_t							__clientBodyBufferSize;

	public:
		Template();
		Template( const Template &copy );
		Template	&operator=( const Template &assign );
		~Template();
};

#endif
