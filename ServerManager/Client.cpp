#include "Client.hpp"

Client::Client()
{

}

Client::Client( String requestLine, String requestHeaders ) :
__requestLine( requestLine ),
__requestHeaders( requestHeaders )
{

}

Client::Client( const Client &copy )
{
	*this = copy;
}

Client::~Client()
{
}

Client	&Client::operator=( const Client &assign )
{
	(void) assign;
	return *this; 
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

String	Client::getHeaderFeildValue( const String &key )
{
	std::map<String, String>::iterator iter = __headerFeilds.find(key);
	if (iter == __headerFeilds.end())
		throw std::exception();
	return iter->second;
}
void	Client::proccessHeaders()
{
	String	tmp(this->__requestHeaders);
	size_t	pos = 0;
	do {
		pos = tmp.find("\r\n");
		if (pos == String::npos && tmp.empty())
			break ;
		else if (pos == String::npos)
			pos = tmp.length();
		{
			String	hf(tmp.begin(), tmp.begin() + pos);
			size_t	p = hf.find(":");
			String	key(hf.begin(), hf.begin() + p);
			String	value(hf.begin() + p + 2, hf.end());
			this->__headerFeilds[key] = value;
			tmp.erase(0, pos + 2);
		}
		
	} while (true);
}
void	Client::proccessRequestLine()
{
	std::istringstream	iss( this->__requestLine );
	String	method, URI, protocole;
	iss >> method;
	iss >> URI;
	iss >> protocole;
	if (method == "OPTIONS") {
		this->__method = OPTIONS;
	} else if (method == "GET") {
		this->__method = GET;
	} else if (method == "HEAD") {
		this->__method = HEAD;
	} else if (method == "POST") {
		this->__method = POST;
	} else if (method == "PUT") {
		this->__method = PUT;
	} else if (method == "DELETE") {
		this->__method = DELETE;
	} else if (method == "TRACE") {
		this->__method = TRACE;
	} else if (method == "CONNECT") {
		this->__method = CONNECT;
	}
	this->__URI = URI;
	this->__protocole = protocole;
	// std::cout << RED << method << "  " << URI << "  " << protocole << RESET << "\n";
}
void	Client::parseRequest()
{
	proccessRequestLine();
	proccessHeaders();
}
