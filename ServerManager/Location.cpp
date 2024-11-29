#include "Location.hpp"

Location::Location()
{

}

Location::Location( String dir ) :
__dirListing( true ),
__rooted( false ),
__directory( dir )
{
	this->__acceptedMethods.push_back( GET );
	this->__acceptedMethods.push_back( HEAD );
	this->__acceptedMethods.push_back( POST );
	this->__acceptedMethods.push_back( DELETE );
}

Location::Location( const Location &copy )
{
	*this = copy;
}

Location::~Location() {
}

Location	&Location::operator=( const Location &assign )
{
	if (this != &assign) {
		this->__rooted = assign.__rooted;
		this->__directory = assign.__directory;
		this->__rootedDir = assign.__rootedDir;
		this->__dirListing = assign.__dirListing;
		this->__defaultFile = assign.__defaultFile;
		this->__subLocations = assign.__subLocations;
		this->__acceptedMethods = assign.__acceptedMethods;
	}
	return *this;
}
/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/
