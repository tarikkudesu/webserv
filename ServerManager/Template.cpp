#include "Template.hpp"

Template::Template( String &line ) :
__line( line ),
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
	*this = copy;
}

Template::~Template()
{

}

Template	&Template::operator=( const Template &assign )
{
	(void) assign;
	return *this;
}

void	Template::addDirective( size_t end )
{
	String	directive = String(__line.begin(), __line.begin() + end);
	WSU::trimSpaces(directive);
	this->__directives.push_back(directive);
	this->__line.erase(0, end + 1);
}
void	Template::addLocationBlock( size_t pos )
{
	size_t end = pos + 1;
	size_t tracker = 1;

	do {
		if (end >= this->__line.length())
			break;
		if (this->__line.at(end) == '}')
			tracker--;
		if (this->__line.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	String locationBlock(this->__line.begin(), this->__line.begin() + end);
	WSU::trimSpaces(locationBlock);
	this->__line.erase(0, end);
}

void	Template::parseDirectives()
{
	do {
		size_t	pos = __line.find_first_of(";{");
		if (pos == String::npos && __line.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error( "invalid block" );
		if (pos == String::npos)
			break ;
		if (__line.at(pos) == ';')
			addDirective( pos );
		else if (__line.at(pos) == '{')
			addLocationBlock( pos );
	} while( true );
}

void	Template::parse()
{
	WSU::trimSpaces(__line);
	__line = __line.substr(1, __line.length() - 2);
	WSU::trimSpaces(__line);
	parseDirectives();
}
