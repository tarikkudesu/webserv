#include "Request.hpp"

Request::Request() : __phase(NEWREQUEST),
                     __bodySize(0)
{
}
Request::Request(const Request &copy)
{
    *this = copy;
}
Request &Request::operator=(const Request &assign)
{
    if (this != &assign)
    {
        this->__URI = assign.__URI;
        this->__method = assign.__method;
        this->__headers = assign.__headers;
        this->__bodySize = assign.__bodySize;
        this->__protocole = assign.__protocole;
        this->__queryString = assign.__queryString;
        this->__headerFeilds = assign.__headerFeilds;
    }
    return *this;
}
Request::~Request()
{
}

/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

void Request::clear()
{
    for (std::vector<s_body>::iterator it = __body.begin(); it != __body.end(); it++)
        it->_headers.clear();
    this->__URI.clear();
    this->__body.clear();
    this->__body.clear();
    this->__bodySize = 0;
    this->__fragement.clear();
    this->__protocole.clear();
    this->__headerFeilds.clear();
    this->__queryString.clear();
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/

void Request::proccessHeaders(String requestHeaders)
{
    size_t pos = 0;
    do
    {
        pos = requestHeaders.find("\r\n");
        if (pos == String::npos)
            break;
        {
            String hf(requestHeaders.begin(), requestHeaders.begin() + pos);
            size_t p = hf.find(": ");
            if (p == String::npos)
                throw ErrorResponse(400, "invalid Header feild");
            String key(hf.begin(), hf.begin() + p);
            wsu::toUpperString(key);
            String value(hf.begin() + p + 2, hf.end());
            if (key.empty() || String::npos != key.find_first_not_of(H_KEY_CHAR_SET))
                throw ErrorResponse(400, "invalid Header feild");
            this->__headerFeilds[key] = value;
            requestHeaders.erase(0, pos + 2);
        }
    } while (requestHeaders.empty() == false);
}
void Request::proccessURI()
{
    size_t start = 0;
    size_t end = 0;
    start = this->__URI.find("?");
    end = this->__URI.find("#");
    if (end != String::npos)
    {
        this->__fragement = String(this->__URI.begin() + end + 1, this->__URI.end());
        this->__URI.erase(end);
    }
    if (start != String::npos)
    {
        this->__queryString = String(this->__URI.begin() + start + 1, this->__URI.end());
        this->__URI.erase(start);
    }
}
void Request::proccessRequestLine(const String &requestLine)
{
    if (2 < std::count(requestLine.begin(), requestLine.end(), ' '))
        throw ErrorResponse(400, "invalid Request Line (extra space)");
    std::istringstream iss(requestLine);
    String method, URI, protocole;
    iss >> method;
    iss >> URI;
    iss >> protocole;
    if (method.empty() || protocole.empty() || URI.empty())
        throw ErrorResponse(400, "invalid Request Line (METHOD URI PROTOCOLE)");
    if (method == "OPTIONS")
        this->__method = OPTIONS;
    else if (method == "GET")
        this->__method = GET;
    else if (method == "HEAD")
        this->__method = HEAD;
    else if (method == "POST")
        this->__method = POST;
    else if (method == "PUT")
        this->__method = PUT;
    else if (method == "DELETE")
        this->__method = DELETE;
    else if (method == "TRACE")
        this->__method = TRACE;
    else if (method == "CONNECT")
        this->__method = CONNECT;
    else
        throw ErrorResponse(501, "invalid method");
    this->__URI = URI;
    if (String::npos != URI.find_first_not_of(URI_CHAR_SET))
        throw ErrorResponse(400, "invalid URI (out of URI CHARSET)");
    this->__protocole = protocole;
    if (this->__protocole != PROTOCOLE_V)
        throw ErrorResponse(505, "Unsupported protocole");
    proccessURI();
}
void Request::parseRequest(String requestLine, String requestHeaders)
{
    clear();
    proccessRequestLine(requestLine);
    proccessHeaders(requestHeaders);
    __headers.parseHeaders(__headerFeilds);
    __headerFeilds.clear();
}

std::ostream &operator<<(std::ostream &o, const Request &req)
{
    std::cout << "Request: \n";
    std::cout << "\tprotocole: " << req.__protocole << "\n";
    std::cout << "\tmethod: " << methodToString(req.__method) << "\n";
    std::cout << "\tURI: " << req.__URI << "\n";
    std::cout << "\tquery: " << req.__queryString << "\n";
    std::cout << "\n";
    std::cout << "\tFragement: " << req.__fragement << "\n";
    std::cout << "\theaders: \n";
    std::cout << req.__headers << "\n";
    std::cout << "\tbody size: " << req.__bodySize << "\n";
    return o;
}
