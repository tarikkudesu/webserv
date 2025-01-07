#include "Response.hpp"

Response::Response(Request &request, Server &server, Location &location) : __server(server),
																		   __request(request),
																		   __location(location),
																		   code(200), ressource(request.__URI),
																		   codeMessage("Ok"), protocole(request.__protocole), body(""),
																		   isDir(false),
																		   isDone(false)
{
	validateMethod();
	// validateRequest();
	isCgi = isValidCgi();
	isFound = fileFound();
	buildResponse();
}

Response::Response(const Response &copy) : __server(copy.__server),
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
	}
	return *this;
}

/***********************************************************************
 *                               METHODS                               *
 ***********************************************************************/

bool Response::validateMethod()
{
	if (__request.__method == GET || __request.__method == POST || __request.__method == DELETE)
		return true;
	throw ErrorResponse(501, "The server does not support the HTTP method used in your request");
}

// bool Response::validateRequest()
// {
// 	if (__request.__protocole != "HTTP/1.1")
// 		throw ErrorResponse(505, "HTTP Version not supported");
// 	if (__request.__host.empty())
// 		throw ErrorResponse(400, "Bad Request");
// 	if (__request.__URI.find("../") != String::npos)
// 		throw ErrorResponse(403, "Forbidden");
// 	if (__request.__method == POST)
// 	{
// 		if (__request.__contentLength == 0)
// 			throw ErrorResponse(411, "Length Required");
// 		if (__request.__contentLength > __location.__clientBodyBufferSize)
// 			throw ErrorResponse(413, "Request Entity Too Large");
// 		if (__request.__requestbody.length() != __request.__contentLength)
// 			throw ErrorResponse(400, "Bad Request");
// 	}
// 	return true;
// }

void Response::setCode(int code)
{
	this->code = code;
}

void Response::setCodeMessage(String message)
{
	this->codeMessage = message;
}

// const std::map<String, String> &Response::getHeaders()
// {
// 	return header;
// }

String Response::buildStartLine(String protocole, int code, String codeMessage)
{
	return protocole + " " + wsu::intToString(code) + " " + codeMessage;
}

bool Response::isValidCgi()
{
	if (__location.__cgiPass.empty())
		return false;
	if (wsu::endWith(ressource, ".java") || wsu::endWith(ressource, ".python"))
		return true;
	return false;
}

void Response::executeCgi()
{
	throw Cgi(__request, __location, ressource);
}

void Response::buildResponse()
{
	if (isDone)
		return;
	if (isCgi && isFound)
		executeCgi();
	else
	{
		switch (__request.__method)
		{
		case GET:
			handleGET();
			break;
		case POST:
			handlePOST();
			break;
		case DELETE:
			handleDELETE();
			break;
		default:
			throw ErrorResponse(405, "Method Not Allowed");
		}
	}
}

String Response::combinePaths(const String &root, const String &path)
{
	String fullPath;

	if (path[0] == '/')
		fullPath = root + path;
	else
		fullPath = root + "/" + path;
	return fullPath;
}

bool Response::isReadable(const char *str)
{
	struct stat fileStat;

	if (stat(str, &fileStat) == 0 && S_ISREG(fileStat.st_mode))
	{
		if (access(str, R_OK) != 0)
			return false;
	}
	else
		return false;
	return true;
}

t_svec Response::listDir(String &dirPath)
{
	t_svec dirs;
	DIR *dir;
	struct dirent *entry;

	dir = opendir(dirPath.c_str());
	if (!dir)
		return (dirs);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue;
		}
		dirs.push_back(entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	std::sort(dirs.begin(), dirs.end());
	return dirs;
}

bool Response::fileFound()
{
	String fullPath = combinePaths(__location.__root, ressource);
	int flag = 0;
	if (dirFound())
	{
		t_svec::iterator it = __location.__index.begin();
		for (; it != __location.__index.end(); it++)
		{
			String indexPath = combinePaths(fullPath, *it);
			if (!isReadable(indexPath.c_str()))
				continue;
			else
			{
				flag = 1;
				break;
			}
		}
		if (flag)
		{
			ressource = combinePaths(ressource, *it);
			return true;
		}
		if (__location.__autoindex)
		{
			autoIndex = listDir(fullPath);
			return true;
		}
		throw ErrorResponse(404, "Ressource not found");
	}
	if (!isReadable(fullPath.c_str()))
		throw ErrorResponse(404, "Ressource not found");
	ressource = fullPath;
	return true;
}

bool Response::dirFound()
{
	String fullPath = combinePaths(__location.__root, ressource.substr(__location.__path.length()));

	struct stat dirStat;
	if (stat(fullPath.c_str(), &dirStat) != 0)
		return false;

	if (!S_ISDIR(dirStat.st_mode))
		return false;

	if (access(fullPath.c_str(), R_OK) != 0)
		return false;

	isDir = true;
	if (ressource[ressource.length() - 1] != '/')
		throw ErrorResponse(301, "Ressource Moved");
	return true;
}

String Response::readFile(const String &fullPath)
{
	std::ifstream f(fullPath.c_str());
	String line;
	String output;

	if (!f.is_open())
		throw ErrorResponse(401, "Unauthorized");
	while (std::getline(f, line))
		!f.eof() ? output += line + "\n" : output += line;
	f.close();
	return output;
}

bool Response::writeFile(const String &path, const String &content)
{
	std::ofstream file(path.c_str());

	if (!file.is_open())
		throw ErrorResponse(401, "Unauthorized");
	file << content;
	file.close();
	return true;
}

bool Response::deleteFile(const String &path)
{
	if (unlink(path.c_str()) != 0)
		throw ErrorResponse(code, codeMessage);
	setCode(204);
	setCodeMessage("No Content");
	return true;
}

void Response::handleGET()
{
	if (isDone)
		return;
	if (isFound && !isCgi)
		body = readFile(ressource);
	else
		throw ErrorResponse(404, "Ressource not found");
}

void Response::handlePOST()
{
	if (isDone)
		return;
	if (isFound && !isCgi)
		writeFile(ressource, __request.__requestbody);
	else
		throw ErrorResponse(404, "Ressource not found");
}

void Response::handleDELETE()
{
	if (isDone)
		return;
	if (isFound && !isCgi)
		deleteFile(ressource);
	else
		throw ErrorResponse(404, "Ressource not found");
}

void Response::buildResponseHeaders()
{
	header["server"] = "webserv/1.0";
	header["date"] = wsu::buildIMFDate();
	header["content_type"] = wsu::getContentType(ressource);
	header["content_length"] = body.length();
}

// void Response::buildResponseBody()
// {
// 	switch (__request.__method)
// 	{
// 	case GET:
// 		if (isAutoIndex)
// 			;
// 		break;

// 	case POST:
// 		break;

// 	case DELETE:
// 		body.clear();
// 		break;

// 	default:
// 		body = "Method not implemented\n";
// 		break;
// 	}
// }

String Response::getStartLine()
{
	return buildStartLine(PROTOCOLE_V, code, codeMessage) + "\r\n";
}

String Response::getResponseBody()
{
	// buildResponseBody();
	return body;
}

// const std::map<String, String> &Response::getHeaders()
// {
// 	buildResponseHeaders();
// 	return header;
// }

String Response::getResponse()
{
	String fullResponse;
	String responseBody = getResponseBody();
	fullResponse = getStartLine();
	// const std::map<String, String> &headers = getHeaders();
	for (std::map<String, String>::const_iterator it = header.begin(); it != header.end(); ++it)
		fullResponse += it->first + ": " + it->second + "\r\n";
	fullResponse += "\r\n";
	if (!responseBody.empty())
		fullResponse += responseBody;
	return fullResponse;
}
