
#include "../ServerManager/ServerManager.hpp"

void	f() {
	int fd = open(".logs/sds.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (-1 == fd)
	{
		WSU::terr( String(" .logs/sds.log cannot be opened") );
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

void signalHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << "exiting\n";
		exit(0);
	}
	WSU::terr(String("SIGPIPE"));
}

int main( int ac, char **av )
{
	atexit(f);
    signal(SIGINT, signalHandler);
    signal(SIGPIPE, signalHandler);
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;
	String	config;
	if (ac == 2)
		config = *(av + 1);
	else if (ac == 1)
		config = "conf/webserv_default.conf";
	ServerManager	webserv( config );
	return 0;
}
