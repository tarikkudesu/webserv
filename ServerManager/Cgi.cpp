/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ooulcaid <ooulcaid@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-07 16:33:08 by ooulcaid          #+#    #+#             */
/*   Updated: 2025-01-07 16:33:08 by ooulcaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Cgi.hpp"

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
}

/*----------------------business logic------------------------*/

void Cgi::execute(const char *bin, const char *path, int fd)
{
	if (dup2(STDOUT_FILENO, fd) < 0)
		exit(1);
	execve(bin, (char *const *)&path, NULL);
	close(fd);
	exit(1);
}

const char *Cgi::getBin(void)
{
	if (wsu::endWith(__explorer.getPath(), ".java"))
		return "/usr/bin/java"; // will be gotten from the config file
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
}

void Cgi::setCgiEnvironement()
{
	/*itterate through the request headers to set them to the process environement*/
	for (mapIterator it = __request.__headerFeilds.begin(); it != __request.__headerFeilds.end(); it++)
		setenv(it->first.c_str(), it->second.c_str(), 0);
	// setenv() not allowed, the code above will be replaced by the argv to give it as argument of the cgi script
	//"(String [] args)in case of java for example"
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

	while (!(child = waitpid(pid, &status, WNOHANG)) && __start - std::clock_t() < TIMEOUT)
		;
	if (!child)
		kill(pid, SIGKILL), throw ErrorResponse(408, __location, "Request Time-out");
	if (WEXITSTATUS(status))
		throw ErrorResponse(500, __location, "internal server error");
	readFromPipe(pip[0]);
	close(pip[0]);
}

/*-----------------------getters----------------------------*/

String &Cgi::getBody()
{
	return __body;
}
