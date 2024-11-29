#include "Response.hpp"

Response::Response()
{

}

Response::Response( const Client &client ) :
__client( client )
{

}

Response::Response( const Response &copy )
{
	(void) copy;
}

Response::~Response()
{
}

Response	&Response::operator=( const Response &assign )
{
	(void) assign;
	return *this;
}

/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/




/***********************************************************************
 *                            STATIC METHODS                           *
 ***********************************************************************/

void Response::replaceString( String &original, const String toReplace, const String replacement ) {
	size_t pos = 0;

	while ((pos = original.find(toReplace, pos)) != String::npos) {
		original.replace(pos, toReplace.length(), replacement);
		pos += replacement.length(); 
	}
}

String	Response::buildErrorPage( int16_t errCode ) {
	if (Response::__errCode.empty()) {
		Response::__errCode.insert(std::make_pair(100, "Continue"));
		Response::__errCode.insert(std::make_pair(101, "Switching Protocols"));
		Response::__errCode.insert(std::make_pair(200, "OK"));
		Response::__errCode.insert(std::make_pair(201, "Created"));
		Response::__errCode.insert(std::make_pair(202, "Accepted"));
		Response::__errCode.insert(std::make_pair(203, "Non-Authoritative Information"));
		Response::__errCode.insert(std::make_pair(204, "No Content"));
		Response::__errCode.insert(std::make_pair(205, "Reset Content"));
		Response::__errCode.insert(std::make_pair(206, "Partial Content"));
		Response::__errCode.insert(std::make_pair(300, "Multiple Choices"));
		Response::__errCode.insert(std::make_pair(301, "Moved Permanently"));
		Response::__errCode.insert(std::make_pair(302, "Found"));
		Response::__errCode.insert(std::make_pair(303, "See Other"));
		Response::__errCode.insert(std::make_pair(304, "Not Modified"));
		Response::__errCode.insert(std::make_pair(305, "Use Proxy"));
		Response::__errCode.insert(std::make_pair(307, "Temporary Redirect"));
		Response::__errCode.insert(std::make_pair(400, "Bad Request"));
		Response::__errCode.insert(std::make_pair(401, "Unauthorized"));
		Response::__errCode.insert(std::make_pair(402, "Payment Required"));
		Response::__errCode.insert(std::make_pair(403, "Forbidden"));
		Response::__errCode.insert(std::make_pair(404, "Not Found"));
		Response::__errCode.insert(std::make_pair(405, "Method Not Allowed"));
		Response::__errCode.insert(std::make_pair(406, "Not Acceptable"));
		Response::__errCode.insert(std::make_pair(407, "Proxy Authentication Required"));
		Response::__errCode.insert(std::make_pair(408, "Request Time-out"));
		Response::__errCode.insert(std::make_pair(409, "Conflict"));
		Response::__errCode.insert(std::make_pair(410, "Gone"));
		Response::__errCode.insert(std::make_pair(411, "Length Required"));
		Response::__errCode.insert(std::make_pair(412, "Precondition Failed"));
		Response::__errCode.insert(std::make_pair(413, "Request Entity Too Large"));
		Response::__errCode.insert(std::make_pair(414, "Request-URI Too Large"));
		Response::__errCode.insert(std::make_pair(415, "Unsupported Media Type"));
		Response::__errCode.insert(std::make_pair(416, "Requested range not satisfiable"));
		Response::__errCode.insert(std::make_pair(417, "Expectation Failed"));
		Response::__errCode.insert(std::make_pair(500, "Internal Server Error"));
		Response::__errCode.insert(std::make_pair(501, "Not Implemented"));
		Response::__errCode.insert(std::make_pair(502, "Bad Gateway"));
		Response::__errCode.insert(std::make_pair(503, "Service Unavailable"));
		Response::__errCode.insert(std::make_pair(504, "Gateway Time-out"));
		Response::__errCode.insert(std::make_pair(505, "HTTP Version not supported"));
	}
	String	errPage( Response::__errPage );
	std::ostringstream	ss;
	ss << errCode;
	Response::replaceString(errPage, "STATUS_CODE", ss.str());
	Response::replaceString(errPage, "REASON_PHRASE", Response::__errCode[errCode]);
	return errPage;
}

std::map< int16_t, String >	Response::__errCode;

String Response::__errPage =	"<!DOCTYPE html>"
								"<html lang=\"en\">"
								"<head>"
								"    <meta charset=\"UTF-8\">"
								"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
								"    <title>STATUS_CODE</title>"
								"</head>"
								"<body style=\"background-color: rgb(17, 23, 50);\">"
								"    <h1 style=\""
								"font-family: sans-serif;"
								"text-align: center;"
								"font-weight: 600;"
								"font-size: 20px;"
								"color: white;"
								"margin-top: 100px;"
								"\">Webserv</h1>"
								"    <h1 style=\""
								"font-family: sans-serif;"
								"text-align: center;"
								"font-weight: 800;"
								"font-size: 200px;"
								"line-height: 20px;"
								"color: rgb(124, 219, 137);"
								"\">STATUS_CODE</h1>"
								"    <h1 style=\""
								"font-family: sans-serif;"
								"text-align: center;"
								"font-weight: 600;"
								"font-size: 40px;"
								"color: white;"
								"margin-top: 100px;"
								"    \">REASON_PHRASE</h1>"
								"</body>"
								"</html>";

