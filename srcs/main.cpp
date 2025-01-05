
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
		ServerManager::up = false;
	}
}

int main(int ac, char **av)
{
	t_svec t;
	t.push_back("1");
	t.push_back("2");
	t.push_back("3");
	t.push_back("4");
	t.push_back("5");
	std::cout << wsu::buildListingBody(t) << "\n";
	exit(1);
	atexit(f);
	signal(SIGINT, signalHandler);
	std::vector<String> args;
	for (int i = 1; i < ac; ++i)
		args.push_back(String(av[i]));
	{
		wsu::logs(args);
		ServerManager webserv(*(args.end() - 1));
		webserv.setUpWebserv();
		ServerManager::clear();
	}
}