#include "./Cgi.hpp"

Cgi::Cgi(/* args */)
{
}


Cgi::Cgi(Request request, String ressource) :
__request(request),
__ressource(ressource),
__body("")
{
    cgiProcess();
}

Cgi::~Cgi()
{
}

bool Cgi::endWith(std::string& file, const char* extension)
{
    int fileLen = file.length();
    int exLen = strlen(extension);

    if (fileLen < exLen)
        return false;

    return file.compare(fileLen - exLen, exLen, extension) == 0;
}

void    Cgi::execute(const char* bin, const char *path, int fd)
{
    if (dup2(STDOUT_FILENO, fd))
    return wsu::error("dup2() syscall failed");
	execve(bin, (char *const *)&path, NULL);
    close(fd);
	wsu::error("execve() syscall failed");
}

const char     *Cgi::getBin(void)
{
    if (endWith(__ressource, ".java"))
        return JAVABIN;
    return PYTHONBIN;
}

String          &Cgi::readFromPipe(int fd)
{
	String str("");
	char buffer[1024];
	int size;
	while ((size = read(fd, buffer, 1023)))
	{
		buffer[size] = '\0';
		str.append(buffer);
	} 
	return str;
}

void            Cgi::cgiProcess(void)
{
    /*itterate through the request headers to set them to the process environement*/
	for (mapIterator it = __request.__headerFeilds.begin(); it != __request.__headerFeilds.end(); it++)
		setenv(it->first.c_str(), it->second.c_str(), 0);
	if (!access(__ressource.c_str(), F_OK))
		return ;
    /*----------------------------------------------------------------------------*/

	int pip[2], pid;
	if (pipe(pip) < 0)
		return wsu::error("pipe() syscall failed");
	pid = fork();
	if (pid < 0)
		return wsu::error("fork() syscall fail");

	if (!pid)
		close(pip[0]), execute(getBin(), __ressource.c_str(), pip[1]);

	close(pip[1]);
	__body = readFromPipe(pip[0]);
    close(pip[0]);
}

String&     Cgi::getBody()
{
    return __body;
}
