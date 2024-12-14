#include "ErrorResponse.hpp"

ErrorResponse::ErrorResponse()
{
}

ErrorResponse::ErrorResponse(int code, String error) : __code(code),
													   __errorIndication(error)
{
	initErrorCodes();
	std::map<int16_t, String>::iterator it = ErrorResponse::__errCode.find(this->__code);
	if (it != ErrorResponse::__errCode.end())
	{
		this->__reasonPhrase = it->second;
	}
	else
	{
		this->__reasonPhrase = "Error";
	}
	this->constructErrorPage();
}

ErrorResponse::ErrorResponse(const ErrorResponse &copy)
{
	(void)copy;
}

ErrorResponse::~ErrorResponse()
{
}

ErrorResponse &ErrorResponse::operator=(const ErrorResponse &assign)
{
	(void)assign;
	return *this;
}
/****************************************************************************
 *                               MINI METHODS                               *
 ****************************************************************************/

const String &ErrorResponse::getResponse()
{
	return this->__finalResponse;
}

/*****************************************************************************
 *                                  METHODS                                  *
 *****************************************************************************/
void ErrorResponse::buildStatusLine()
{
	std::stringstream ss;
	ss << this->__code;
	this->__StatusLine = PROTOCOLE_V " " + ss.str() + " " + this->__reasonPhrase + "\r\n";
}
void ErrorResponse::buildResponseBody()
{
	this->__Body = ErrorResponse::__errPage;
	std::stringstream ss;
	ss << this->__code;
	WSU::replaceString(this->__Body, "CODE", ss.str());
	WSU::replaceString(this->__Body, "REASON_PHRASE", this->__reasonPhrase);
	WSU::replaceString(this->__Body, "MESSAGE", this->__errorIndication);
}
void ErrorResponse::buildHeaderFeilds()
{
	this->__headers += "Accept-Ranges: none\r\n";
	this->__headers += "Connection: keep-alive\r\n";
	size_t conLen = this->__Body.length();
	std::stringstream ss;
	ss << conLen;
	this->__headers += "Content-Length: " + ss.str() + "\r\n";
	this->__headers += "Content-Type: text/html; charset=UTF-8\r\n";
	this->__headers += "Server: Webserv\r\n";
	this->__headers += "Date: " + WSU::buildIMFDate() + "\r\n";
	this->__headers += "\r\n";
}
void ErrorResponse::constructErrorPage()
{
	buildStatusLine();
	buildResponseBody();
	buildHeaderFeilds();
	this->__finalResponse = this->__StatusLine + this->__headers + this->__Body;
}
/***********************************************************************
 *                            STATIC METHODS                           *
 ***********************************************************************/

std::map<int16_t, String> ErrorResponse::__errCode;

String ErrorResponse::__errPage = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">\n"
								  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
								  "<title>Webserv</title></head><body style=\"background-color: rgb(35, 40, 47);\">\n"
								  "<div style=\"border: 1px solid rgba(210, 215, 223, 0.26); border-radius: 4px; margin-top: 100px; background-color: rgb(22, 27, 34);\">\n"
								  "<h1 style=\"font-size: 80px; font-family: sans-serif; text-align: center; padding: 20px 0px 0px 0px; margin: 0px; color: rgb(210, 215, 223);\">\n"
								  "CODE\n"
								  "</h1><h2 style=\"font-size: 25px; font-family: sans-serif; text-align: center; padding: 0px 0px 10px 0px; margin: 0px; color: rgb(210, 215, 223);\">\n"
								  "REASON_PHRASE\n"
								  "</h2><p style=\"font-size: 16px; font-family: sans-serif; text-align: center; padding: 0px 0px 30px 0px; margin: 0px; color: rgb(90, 139, 223);\">\n"
								  "MESSAGE\n"
								  "</p></div></body></html>";

void ErrorResponse::initErrorCodes()
{
	if (ErrorResponse::__errCode.empty())
	{
		ErrorResponse::__errCode.insert(std::make_pair(100, "Continue"));
		ErrorResponse::__errCode.insert(std::make_pair(101, "Switching Protocols"));
		ErrorResponse::__errCode.insert(std::make_pair(200, "OK"));
		ErrorResponse::__errCode.insert(std::make_pair(201, "Created"));
		ErrorResponse::__errCode.insert(std::make_pair(202, "Accepted"));
		ErrorResponse::__errCode.insert(std::make_pair(203, "Non-Authoritative Information"));
		ErrorResponse::__errCode.insert(std::make_pair(204, "No Content"));
		ErrorResponse::__errCode.insert(std::make_pair(205, "Reset Content"));
		ErrorResponse::__errCode.insert(std::make_pair(206, "Partial Content"));
		ErrorResponse::__errCode.insert(std::make_pair(300, "Multiple Choices"));
		ErrorResponse::__errCode.insert(std::make_pair(301, "Moved Permanently"));
		ErrorResponse::__errCode.insert(std::make_pair(302, "Found"));
		ErrorResponse::__errCode.insert(std::make_pair(303, "See Other"));
		ErrorResponse::__errCode.insert(std::make_pair(304, "Not Modified"));
		ErrorResponse::__errCode.insert(std::make_pair(305, "Use Proxy"));
		ErrorResponse::__errCode.insert(std::make_pair(307, "Temporary Redirect"));
		ErrorResponse::__errCode.insert(std::make_pair(400, "Bad Request"));
		ErrorResponse::__errCode.insert(std::make_pair(401, "Unauthorized"));
		ErrorResponse::__errCode.insert(std::make_pair(402, "Payment Required"));
		ErrorResponse::__errCode.insert(std::make_pair(403, "Forbidden"));
		ErrorResponse::__errCode.insert(std::make_pair(404, "Not Found"));
		ErrorResponse::__errCode.insert(std::make_pair(405, "Method Not Allowed"));
		ErrorResponse::__errCode.insert(std::make_pair(406, "Not Acceptable"));
		ErrorResponse::__errCode.insert(std::make_pair(407, "Proxy Authentication Required"));
		ErrorResponse::__errCode.insert(std::make_pair(408, "Request Time-out"));
		ErrorResponse::__errCode.insert(std::make_pair(409, "Conflict"));
		ErrorResponse::__errCode.insert(std::make_pair(410, "Gone"));
		ErrorResponse::__errCode.insert(std::make_pair(411, "Length Required"));
		ErrorResponse::__errCode.insert(std::make_pair(412, "Precondition Failed"));
		ErrorResponse::__errCode.insert(std::make_pair(413, "Request Entity Too Large"));
		ErrorResponse::__errCode.insert(std::make_pair(414, "Request-URI Too Large"));
		ErrorResponse::__errCode.insert(std::make_pair(415, "Unsupported Media Type"));
		ErrorResponse::__errCode.insert(std::make_pair(416, "Requested range not satisfiable"));
		ErrorResponse::__errCode.insert(std::make_pair(417, "Expectation Failed"));
		ErrorResponse::__errCode.insert(std::make_pair(500, "Internal Server Error"));
		ErrorResponse::__errCode.insert(std::make_pair(501, "Not Implemented"));
		ErrorResponse::__errCode.insert(std::make_pair(502, "Bad Gateway"));
		ErrorResponse::__errCode.insert(std::make_pair(503, "Service Unavailable"));
		ErrorResponse::__errCode.insert(std::make_pair(504, "Gateway Time-out"));
		ErrorResponse::__errCode.insert(std::make_pair(505, "HTTP Version not supported"));
	}
}
