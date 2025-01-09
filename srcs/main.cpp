
#include "../ServerManager/ServerManager.hpp"

ServerManager *webservP = NULL;

void f()
{
	int fd = open(".logs/sds.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (-1 == fd)
	{
		wsu::terr(String(" .logs/sds.log cannot be opened"));
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	system("lsof -c webserv");
	std::cout << "\n********************************************************************\n";
	std::cout << "********************************************************************\n\n";
	system("leaks -list webserv");
}

void signalHandler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "exiting\n";
		Core::up = false;
	}
}

int main(int ac, char **av)
{
	atexit(f);
	signal(SIGINT, signalHandler);
	std::vector<String> args;
	for (int i = 1; i < ac; ++i)
		args.push_back(String(av[i]));
	{
		wsu::logs(args);
		ServerManager webserv(*(args.end() - 1));
		webserv.setUpWebserv();
	}
}