#include "Response.hpp"

Response::Response(Request &request,
				   Server &server,
				   Location &location) : explorer(RessourceHandler(location, request.__URI)),
										 __server(server),
										 __request(request),
										 __location(location),
										 code(200),
										 token(Token(__request.__body, server.__tokenDB))

{
	std::vector< t_method >::iterator it = __location.__allowMethods.begin();
	for (; it != __location.__allowMethods.end() && *it != __request.__method; it++)
		;
	if (it == __location.__allowMethods.end())
		throw ErrorResponse(405, __location, getMethod(__request.__method) + " : method not allowed in this location");
	__check_methods();
	buildResponse();
}

String Response::getMethod(int Method)
{
	switch (Method)
	{
		case GET:
			return "GET";
		case POST:
			return "POST";
		default:
			return "DELETE";
	}
}

Response::Response(const Response &copy) : explorer(RessourceHandler(copy.explorer)),
										   __server(copy.__server),
										   __request(copy.__request),
										   __location(copy.__location),
										token(copy.token)

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
		this->reasonPhrase = assign.reasonPhrase;
		this->explorer = assign.explorer;
		this->headers = assign.headers;
		this->body = assign.body;
	}
	return *this;
}

/***********************************************************************
 *								 METHODS							   *
 ***********************************************************************/

bool Response::checkCgi()
{
	if (__location.__cgiPass.empty())
		return 0;
	if (explorer.__type == FOLDER)
		return 0;
	if (!wsu::endWith(explorer.__fullPath, ".java") && !wsu::endWith(explorer.__fullPath, ".php"))
		return 0;
	return 1;
}

bool	Response::shouldAuthenticate()
{
	return !__location.__authenticate.empty();
}

bool	Response::authenticated()
{
	if (__request.__headers.__cookie.empty())
		return false;
	t_svec cookies = wsu::splitByChar(__request.__headers.__cookie, ';');
	for (t_svec::iterator it = cookies.begin(); it != cookies.end(); it++)
	{
		t_svec cook = wsu::splitByChar(*it, '=');
		if (cook.size() == 2 && token.authentified(cook[1]))
			return true;
	}
	return false;
}

void Response::__check_methods()
{
	if (checkCgi())
		executeCgi();
	else if (__request.__method == GET)
		executeGet();
	else if (__request.__method == POST)
		executePost();
	else if (__request.__method == DELETE)
		executeDelete();
	else
		throw ErrorResponse(405, __location, "Server does not implement this method");
}

void Response::executeCgi()
{
	Cgi cgi(explorer, __request, __location);
	body.push_back(cgi.getBody());
}

void Response::executeGet()
{
	if (shouldAuthenticate() && !authenticated())
	{
		Location location(__location); //we shouldnt update the location of the config
		location.__index.clear();
		location.__index.push_back(__location.__authenticate);
		RessourceHandler tmpExplorer(location, __request.__URI);
		Get get(location.__autoindex, tmpExplorer, body);
	}
	else
		Get get(__location.__autoindex, explorer, body);
	reasonPhrase = "Ok";
	code = 200;
}

String	readFielContent(String fileName)
{
    String userInfo;
    String buffer;
    std::ifstream file(fileName.c_str());
	
    while (std::getline(file, buffer))
        userInfo.append(buffer);
	return userInfo;
}

void Response::executePost()
{
	//verify if the post content shouldnt be reconstructed;  
	if (__request.__headers.__transferType != MULTIPART)
	{
		String cook = readFielContent(__request.__body[0]._fileName);
		if (!token.authentified(cook))
			cook = token.addUserInDb(cook, __server.serverIdentity());
		// if (cook.empty())
		// 	throw ErrorResponse(301, __location.__authenticate, __location);
		throw ErrorResponse(explorer.__fullPath, cook);
	}
	Post post(explorer, __request);
	code = 200;
	reasonPhrase = "Ok";
}

void Response::executeDelete()
{
	Delete _delete(explorer);
	code = 204;
	reasonPhrase = "No Content";
}

void Response::buildResponse()
{
	setHeader();
	BasicString resMsg = PROTOCOLE_V " " + wsu::intToString(code) + " " + reasonPhrase + "\r\n";
	for (std::map<String, String>::iterator it = headers.begin(); it != headers.end(); ++it)
		resMsg.join(it->first + ": " + it->second + "\r\n");
	resMsg.join(String("\r\n"));
	body.insert(body.begin(), resMsg);
}

void Response::setHeader()
{
	headers["Accept-Ranges"] = "none";
	headers["Connection"] = "keep-alive";
	size_t len = 0;
	for (std::vector<BasicString>::iterator it = body.begin(); it != body.end(); it++)
		len += it->length();
	headers["content-length"] = wsu::intToString(len);
	headers["Content-Type"] = wsu::getContentType(explorer.getPath()) + "; charset=UTF-8";
	headers["server"] = "webserv/1.0";
	headers["date"] = wsu::buildIMFDate();
	if (!__request.__headers.__cookie.empty())
		headers["cookie"] = "token=" + __request.__headers.__cookie + "; expires=Thu, 31 Dec 2025 12:00:00 UTC;";
}

const std::vector<BasicString> &Response::getResponse() const
{
	return body;
}

void Response::print() const
{
	std::cout << GREEN << "*******************************************************************\n";
	std::cout << explorer;
	for (std::vector<BasicString>::const_iterator it = body.begin(); it != body.end(); it++)
		std::cout << GREEN << *it << "\n";
	std::cout << "*****************************Response******************************\n"
			  << RESET;
}
std::ostream &operator<<(std::ostream &o, const Response &r)
{
	r.print();
	return o;
}
