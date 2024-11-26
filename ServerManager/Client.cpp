#include "Client.hpp"

Client::Client(/* args */)
{
}

Client::Client(int fd) : 
fd(fd),
buffer(""),
status(200)
{
	(void)this->fd;
}

Client::~Client()
{
}

Client::Client(const Client& client)
{
	*this = client;
}
Client& Client::operator=(const Client& client)
{
	if (this != &client)
		this->status = client.status;
		//other things to copy deeply
	return *this; 
}

void	Client::setStatus(short status)
{
	this->status = status;
}

short Client::getStatus(void)
{
	return this->status;
}

String Client::getBuffer(void)
{
	return this->buffer;
}

void    Client::setBuffer(String buffer)
{
	this->buffer = buffer;
}

void Client::appendBuffer(String buffer)
{
	this->buffer += buffer;
}

void Client::erase(int pos)
{
	this->buffer.erase(0, pos);
}

String Client::getPath(void)
{
	return path;  // Return the request path
}

void Client::setPath(String path)
{
	this->path = path;  // Set the request path
}

String Client::getBody(void)
{
	return requestBody;  // Return the body of the request
}

void Client::setBody(String body)
{
	this->requestBody = body;  // Set the body of the request
}

String	Client::getHeader(String key)
{
	if (headers.find(key) != headers.end())
		return headers[key];
	return "";
}

void Client::setHeader(String key, String value)
{
	this->headers[key] = value;  // Set the headers map
}

void Client::setQueryVars(std::map<String, String>& query)
{
	this->queryVars = query;  // Set the query variables map
}

std::map<String, String> Client::getQueryVars(void)
{
	return this->queryVars;
}
