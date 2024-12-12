#include "Template.hpp"

Template::Template() :
__host( "0.0.0.0" ),
__root( "./Content" ),
__index( "index.html" ),
__clientBodyBufferSize( 8000 )
{
	__ports.push_back( 8080 );
	__allowMethods.push_back( GET );
	__allowMethods.push_back( POST );
	__allowMethods.push_back( DELETE );
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
