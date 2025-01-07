#include "Response.hpp"

Response::Response(Request &request,
			Server &server,
			Location &location) : __server(server),
						__request(request),
						__location(location),
						explorer(RessourceHandler(location, request.__URI))
{
	__check_methods();
	buildResponse();
}

Response::Response(const Response &copy) : __server(copy.__server),
					__request(copy.__request),
					__location(copy.__location),
					explorer(RessourceHandler(copy.explorer))
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

void	Response::__check_methods()
{
	if (!__location.__cgiPass.empty())
		executeCgi();
	else
	{
		switch (__request.__method)
		{
		case	GET : executeGet();
			break;
		case	POST : executePost();
			break;
		case	DELETE : executeDelete();
			break;
		default: throw ErrorResponse(405, "Method Not Allowed");
			break;
		}
	}
}

void	Response::executeCgi()
{
	Cgi cgi(explorer, __request, __location);
	body = cgi.getBody();
}


void	Response::executeGet()
{
	Get get(__location.__autoindex ,explorer);
	body = get.getBody();
	code = 200;
	reasonPhrase = "Ok";
}

void	Response::executePost()
{
	Post post(explorer, __request.__requestbody);
	body.clear();
	code = 200;
	reasonPhrase = "Ok";
}

void	Response::executeDelete()
{
	Delete _delete(explorer);
	code = 204;
	reasonPhrase = "No Content";
}

void	Response::buildResponse()
{
	fullResponse = PROTOCOLE_V" " + wsu::intToString(code) + " " + reasonPhrase + "\r\n";
	for (std::map<String, String>::iterator it = headers.begin(); it != headers.end(); ++it)
		fullResponse += it->first + ": " + it->second + "\r\n";
	fullResponse += "\r\n";
	fullResponse += body;
}

void	Response::setHeader()
{
	headers["server"] = "webserv/1.0";
	headers["date"] = wsu::buildIMFDate();
	headers["content-type"] = "webserv/1.0";
	headers["content-length"] = wsu::intToString(body.length());
}

String	Response::getResponse()
{
	return fullResponse;
}
