#include "Response.hpp"

Response::Response(Request &request, Server &server, Location &location) : __server(server),
									   __request(request),
									   __location(location),
									   ressource(request.__URI), isCgi(false), isDir(false), isDone(false), code(200), codeMessage("Ok"),
									   protocole(request.__protocole), body("")
{
	if (!validateMethod() || !validateRequest())
		throw ErrorResponse(code, codeMessage);
	if (!location.__cgiPass.empty())
		isCgi = true;
	if (fileFound())
		isFound = true;
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

bool	Response::validateMethod()
{
	if (__request.__method == GET || __request.__method == POST || __request.__method == DELETE)
		return true;
	return false;
}

bool	Response::validateRequest()
{
	if (__request.__protocole != "HTTP/1.1")
	{
        	setCode(505);
		setCodeMessage("HTTP Version not supported");
		return false;
	}
	if (__request.__host.empty())
	{
        	setCode(400);
		setCodeMessage("Bad Request");
		return false;
	}
	if (__request.__URI.find("../") != String::npos)
	{
        	setCode(403);
		setCodeMessage("Forbidden");
        	return false;
    	}
    	if (__request.__method == POST)
	{
        	if (__request.__contentLength == 0) 
		{
            		setCode(411);
			setCodeMessage("Length Required");
            		return false;
        	}
        	if (__request.__contentLength > __location.__clientBodyBufferSize)
		{
        	    	setCode(413);
			setCodeMessage("Request Entity Too Large");
        	    	return false;
        	}
        	if (__request.__requestbody.length() != __request.__contentLength)
		{
        	    	setCode(400);
			setCodeMessage("Bad Request");
        	    	return false;
        	}
    	}
	return true;
}

void	Response::setCode( int code )
{
	this->code = code;
}

void	Response::setCodeMessage( String message )
{
	this->codeMessage = message;
}

void	Response::init_headers()
{
	header["content_type"] = content_type;
	header["content_length"] = content_length;
}

const std::map<String, String>& 	Response::getHeaders() const
{
	return header;
}

String	Response::buildStartLine( String protocole, int code, String codeMessage )
{
	return protocole + " " + std::to_string(code) + " " + codeMessage;
}

void	Response::handleError( int errorCode )
{

}

// void executeCgi()
// {
// 	Cgi cgi =  new  Cgi(ressource, __request);
// 	body = cgi.getBody();

// }

void	Response::buildResponse()
{
	if (isDone)
		return ;
	if (isCgi && isFound)
	{
		executeCgi();
	}
	else
	{
		if (__request.__method == GET)
			handleGET();
		else if (__request.__method == POST)
			handlePOST();
		else if (__request.__method == DELETE)
			handleDELETE();
	}
}
  
String	Response::combinePaths( const String &root, const String &path )
{
	String fullPath;

	if (path[0] == '/')
		fullPath = root + path;
	else
		fullPath = root + "/" + path;
	return fullPath;
}

bool Response::isReadable( const char *str )
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

t_svec	Response::listDir( String &dirPath )
{
	t_svec	dirs;
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(dirPath.c_str());
	if (!dir)
		return (perror("opendir"), "");
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		dirs.push_back(entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	std::sort(dirs.begin(), dirs.end());
	return dirs;
}

bool	Response::fileFound()
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
				break ;
			}
		}
		if (flag)
		{
			ressource = combinePaths(ressource, *it);
			return true;
		}
		if (__location.__autoindex)
		{
			autoIndex = listDir( fullPath );
			return true;
		}
		setCode(404);
		setCodeMessage("Ressource not found");
		return false;
	}
	struct stat	fileStat;

	if (!isReadable(fullPath.c_str()))
	{
		setCode(404);
		setCodeMessage("Ressource not found");
		return false;
	}
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
	{
		setCode(401);
		setCodeMessage("Unauthorized");
    		return false;
	}

    	isDir = true;
    	if (ressource[ressource.length() - 1] != '/')
    	{
    	    	setCode(301);
    	    	header["Location"] = ressource + "/";
    	    	return false;
    	}
    	return true;
}

String	Response::readFile( const String &fullPath )
{
	std::ifstream	f(fullPath);
	String 		line;
	String		output;

	while (std::getline(f, line))
	{
		output += line;
		output += '\n';
	}
	if (!output.empty() && output.back() == '\n')
        	output.pop_back();
}

void	Response::handleGET()
{
	if (isDone)
		return ;
	if (isFound && !isCgi)
		readFile(ressource);
	else
	{
		setCode(404);
		setCodeMessage("Ressource not found");
	}
}

void	Response::handlePOST()
{
	if (isDone)
		return ;
	
	
}

void	Response::handleDELETE()
{
	if (isDone)
		return ;
	
}

void	Response::buildResponseHeaders()
{

}

void	Response::buildResponseBody()
{

}


String	Response::buildFullResponse()
{
	String	fullResponse;

	fullResponse = buildStartLine(__request.__protocole, code, codeMessage);
	buildResponseHeaders();
	buildResponseBody();

	return fullResponse;
}
