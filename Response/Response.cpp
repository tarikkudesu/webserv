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
    this->fullResponse = assign.fullResponse;
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

void Response::__check_methods()
{
  if (!__location.__cgiPass.empty())
    executeCgi();
  else
  {
    switch (__request.__method)
    {
    case GET:
      executeGet();
      break;
    case POST:
      // throw ErrorResponse(500, "still working on it");
      executePost();
      break;
    case DELETE:
      throw ErrorResponse(500, "still working on it");
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
  body = cgi.getBody();
}

void Response::executeGet()
{
  explorer.loadPathExploring();
  Get get(__location.__autoindex, explorer);
  body = get.getBody();
  code = 200;
  reasonPhrase = "Ok";
}

void Response::executePost()
{
  Post post(explorer, __request);
  body.clear();
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
  fullResponse = PROTOCOLE_V " " + wsu::intToString(code) + " " + reasonPhrase + "\r\n";
  for (std::map<String, String>::iterator it = headers.begin(); it != headers.end(); ++it)
    fullResponse += it->first + ": " + it->second + "\r\n";
  fullResponse += "\r\n";
  fullResponse += body;
}

void Response::setHeader()
{
  headers["Accept-Ranges"] = "none";
  headers["Connection"] = "keep-alive";
  headers["content-length"] = wsu::intToString(body.length());
  headers["Content-Type"] = wsu::getContentType(explorer.getPath()) + "; charset=UTF-8";
  headers["server"] = "webserv/1.0";
  headers["date"] = wsu::buildIMFDate();
}

String Response::getResponse() const
{
  return fullResponse;
}

void Response::print() const
{
  std::cout << GREEN << "*******************************************************************\n";
  std::cout << explorer;
  std::cout << GREEN << fullResponse << "\n";
  std::cout << "*****************************Response******************************\n"
            << RESET;
}
std::ostream &operator<<(std::ostream &o, const Response &r)
{
  r.print();
  return o;
}
