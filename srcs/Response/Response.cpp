#include "Response.hpp"

Response::Response(Request &request,
                   Server &server,
                   Location &location) : explorer(RessourceHandler(location, request.__URI)),
                                         __server(server),
                                         __request(request),
                                         __location(location)

{
    if (wsu::__criticalOverLoad == true)
        throw ErrorResponse(503, "critical server overload");
    __check_methods();
    buildResponse();
}

Response::Response(const Response &copy) : explorer(RessourceHandler(copy.explorer)),
                                           __server(copy.__server),
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
        this->reasonPhrase = assign.reasonPhrase;
        this->explorer = assign.explorer;
        this->headers = assign.headers;
        this->body = assign.body;
    }
    return *this;
}

/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/

bool Response::checkCgi()
{
    if (!__location.__cgiPass.empty())
        return 0;
    if (explorer.__type == FOLDER)
        return 0
        if (!wsu::endWith(explorer.__fullPath, ".java") && !wsu::endWith(explorer.__fullPath, ".php"))
            return 0;
        return 1;
}

void Response::__check_methods()
{
    if (checkCgi())
        executeCgi();
    else
    {
        switch (__request.__method)
        {
        case GET:
            executeGet();
            break;
        case POST:
            executePost();
            break;
        case DELETE:
            executeDelete();
            break;
        default:
            throw ErrorResponse(405, __location, "Server does not implement this method");
            break;
        }
    }
}

void Response::executeCgi()
{
    Cgi cgi(explorer, __request, __location);
    body.push_back(cgi.getBody());
}

void Response::executeGet()
{
    explorer.loadPathExploring();
    Get get(__location.__autoindex, explorer, body);
    code = 200;
    reasonPhrase = "Ok";
}

void Response::executePost()
{
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
