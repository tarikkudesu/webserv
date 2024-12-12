#include "Template.hpp"

Template::Template()
{

}

Template::Template( const Template &copy )
{

	(void) copy;
}

Template::~Template()
{

}

Template	&Template::operator=( const Template &assign )
{
	(void) assign;
	return *this;
}
