#include "Config.hpp"

Config::Config( String &configuration_file )
{
	(void)configuration_file;
}

Config::Config( const Config &copy )
{
	(void) copy;
}

Config::~Config()
{

}

Config	&Config::operator=( const Config &assign )
{
	(void) assign;
	return *this;
}
