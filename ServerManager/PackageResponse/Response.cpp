#include "Response.hpp"

Response::Response(Request &request, Server &server, Location &location) : __server(server),
																		   __request(request),
																		   __location(location)
{
}

Response::Response(const Response &copy) : __server(copy.__server),
										   __request(copy.__request),
										   __location(copy.__location)
{
	*this = copy;
}

Response::~Response()
{
}

Response &Response::operator=(const Response &assign)
{
	if (this != &assign)
	{
		this->__server = assign.__server;
		this->__request = assign.__request;
		this->__location = assign.__location;
	}
	return *this;
}

/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/
