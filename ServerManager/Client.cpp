#include "Client.hpp"

Client::Client() :
__transferEncoding( GENERAL ),
__connectionType( KEEP_ALIVE ),
__contentLength( 0 )
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
	if (this != &assign) {
		this->__URI = assign.__URI;
		this->__method = assign.__method;
		this->__protocole = assign.__protocole;
		this->__headerFeilds = assign.__headerFeilds;
		this->__contentLength = assign.__contentLength;
		this->__connectionType = assign.__connectionType;
		this->__transferEncoding = assign.__transferEncoding;
	}
	return *this; 
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void	Client::clear()
{
	this->__URI.clear();
	this->__protocole.clear();
	this->__headerFeilds.clear();
	this->__transferEncoding = GENERAL;
}
bool	Client::connectionTypeClose()
{
	if (this->__connectionType == CLOSE)
		return true;
	return false;
}
String	Client::getHeaderFeildValue( const String &key )
{
	std::map<String, String>::iterator iter = __headerFeilds.find(key);
	if (iter == __headerFeilds.end())
		throw std::exception();
	return iter->second;
}
size_t	Client::getContentLength()
{
	return this->__contentLength;
}
t_transferEncoding	Client::gettransferEncoding()
{
	return this->__transferEncoding;
}
t_connectionType	Client::getconnectionType()
{
	return this->__connectionType;
}
bool	Client::hasBody()
{
	if (this->__transferEncoding == CHUNKED)
		return true;
	else if (this->__contentLength)
		return true;
	return false;
}
void	Client::setBody( const String &body )
{
	this->__requestbody = body;
}
/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/
void	Client::contentLength()
{
	try {
		std::istringstream	ss(getHeaderFeildValue("Content-Length"));
		ss >> this->__contentLength;
	} catch ( std::exception &e ) {}
}
void	Client::connectionType()
{
	try {
		String value = getHeaderFeildValue("connection");
		if (value == "close")
			this->__connectionType = KEEP_ALIVE;
	} catch ( std::exception &e ) {}
}
void	Client::transferEncoding()
{
	try {
		String value = getHeaderFeildValue("Transfer-Encoding");
		if (value.find("chunked") != String::npos)
			this->__transferEncoding = CHUNKED;
	} catch ( std::exception &e ) {}
}
void	Client::proccessHeaders( String requestHeaders )
{
	size_t	pos = 0;
	do {
		pos = requestHeaders.find("\r\n");
		if (pos == String::npos)
			break ;
		{
			String	hf(requestHeaders.begin(), requestHeaders.begin() + pos);
			size_t	p = hf.find(":");
			String	key(hf.begin(), hf.begin() + p);
			String	value(hf.begin() + p + 2, hf.end());
			std::cout << BLUE << key << RESET << ":= " << GREEN << value << RESET << "\n";
			this->__headerFeilds[key] = value;
			requestHeaders.erase(0, pos + 2);
		}
		
	} while (requestHeaders.empty() == false);
}
void	Client::proccessRequestLine( const String &requestLine )
{
	std::istringstream	iss( requestLine );
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
}
void	Client::parseRequest( const String &requestLine, const String &requestHeaders )
{
	clear();
	proccessRequestLine(requestLine);
	proccessHeaders(requestHeaders);
	transferEncoding();
	connectionType();
	contentLength();
}
