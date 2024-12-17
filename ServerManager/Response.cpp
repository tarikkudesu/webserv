#include "Response.hpp"

Response::Response(Request &request, Server &server) : __request(request), __server(server)
{
	build();
}

Response::Response(const Response &copy) : __request(copy.__request), __server(copy.__server)
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
		this->__request = assign.__request;
		this->__server = assign.__server;
	}
	return *this;
}

/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/

void Response::build()
{

}
