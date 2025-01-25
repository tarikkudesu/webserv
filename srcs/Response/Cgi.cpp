/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ooulcaid <ooulcaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:33:08 by ooulcaid          #+#    #+#             */
/*   Updated: 2025/01/25 17:21:40 by ooulcaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

typedef std::map<String, String> Map;

/*=---------------------constructors-----------------------*/

Cgi::Cgi(RessourceHandler &explorer,
		 Request &request,
		 Location &location) : __request(request),
							   __explorer(explorer),
							   __location(location),
							   __start(std::clock_t()),
							   __body("")

{
	cgiProcess();
}

Cgi::~Cgi()
{
	clear();
}

/*----------------------business logic------------------------*/

void Cgi::clear( void )
{
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	delete [] env;
}

void Cgi::execute(const char *bin, const char *path, int fd)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		clear();
		throw ErrorResponse(500, __location, "internal server error");
		exit(1);
	}
	const char *argv[] = {bin, path, NULL};
	execve(bin, (char *const *)argv, env);
	clear();
	close(fd);
	throw ErrorResponse(500, __location, "internal server error");
	exit(1);
}

const char *Cgi::getBin(void)
{
	if (wsu::endWith(__explorer.getPath(), ".java"))
		return "/usr/bin/java";
	if (wsu::endWith(__explorer.getPath(), ".php"))
		return "/usr/bin/php";
	return "/usr/bin/java";
}

void Cgi::readFromPipe(int fd)
{
	char buffer[1024] = {0};
	while (read(fd, buffer, 1023))
	{
		__body.append(buffer);
		bzero(buffer, 1024);
	}
	size_t pos = __body.find('\n');
	__body = __body.substr(pos + 1);
}

const char *Cgi::getMethod()
{
	if (__request.__method == POST)
		return "POST";
	return "GET";
}

String	Cgi::getQueryString()
{
	std::string queryString;
	if (__request.__method == POST)
	{
		char buffer[READ_SIZE];
		std::ifstream reader(__request.__body[0]._fileName.c_str());
		do 
		{
			reader.read(buffer, READ_SIZE);
			queryString.append(buffer);
			bzero(buffer, READ_SIZE);
		} while (reader.gcount());
		return queryString;
	}
	return __request.__queryString; 
}

void Cgi::setCgiEnvironement()
{
	Map headers = __request.__headerFeilds;
	headers["GATEWAY_INTERFACE"] = "CGI/1.1";
	headers["SERVER_NAME"] = "SERVER_NAME";//tmp header value
	headers["SERVER_SOFTWARE"] = "WebServ-1337/1.0.0";
	headers["SERVER_PROTOCOL"] = "HTTP/1.1";
	headers["SERVER_PORT"] = "9001"; //tmp header value
	headers["QUERY_STRING"] = getQueryString();
	headers["REQUEST_METHOD"] = getMethod();
	headers["SCRIPT_NAME"] = "index.php";
	headers["SCRIPT_FILENAME"] = "cgi-bin/php/index.php";
	headers["REDIRECT_STATUS"] = "200";
	headers["REMOTE_ADDR"] = "127.0.0.1";//tmp header value
	headers["REMOTE_HOST"] = "127.0.0.1";//tmp header value
	env = new char*[headers.size() + 1]; 
	int i = 0;
	for (Map::iterator it = headers.begin(); it != headers.end(); it++, i++)
	{
		String header = it->first + "=" + it->second;
		env[i] = new char[header.size() + 1];
		std::strcpy(env[i], header.c_str());
	}
	env[i] = NULL;
}

void Cgi::cgiProcess(void)
{
	setCgiEnvironement();

	int child, status, pip[2], pid;
	if (pipe(pip) < 0)
		throw ErrorResponse(500, __location, "internal server error");

	pid = fork();
	if (pid < 0)
		throw ErrorResponse(500, __location, "internal server error");

	if (!pid)
		close(pip[0]), execute(getBin(), __explorer.getPath().c_str(), pip[1]);

	close(pip[1]);

	while (!(child = waitpid(pid, &status, WNOHANG)) && (__start - std::clock_t()) / CLOCKS_PER_SEC < TIMEOUT)
		;
	if (!child)
		kill(pid, SIGKILL), throw ErrorResponse(408, __location, "Request Time-out");
	if (WIFEXITED(pid) && WEXITSTATUS(status))
			throw ErrorResponse(500, __location, "internal server error");
	readFromPipe(pip[0]);
	close(pip[0]);
}

/*-----------------------getters----------------------------*/

String &Cgi::getBody()
{
	return __body;
}
