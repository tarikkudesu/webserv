
#include "../ServerManager/ServerEngine.hpp"

void	f() {
	int fd = open(".logs/sds.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (-1 == fd)
	{
		Logs::terr( String(" .logs/sds.log cannot be opened") );
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

int main( int ac, char **av )
{
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;
	/**********************************
	 * ENABLE INSTANT BUFFER FULSHING *
	 **********************************/
	atexit(f);
	/******************************************************************
	 * CHECK .logs/sds.log FOR MEMORY LEAKS AND FILE DESCRIPTOR LEAKS *
	 ******************************************************************/

	if (ac > 2) {
		Logs::terr( String("wrong argument list") );
	} else if (ac == 2) {
		ServerEngine	webserv( *(av + 1) );
		webserv.setUpWebserv();
		webserv.mainLoop();
	} else {
		ServerEngine	webserv( "default.conf" );
		webserv.setUpWebserv();
		webserv.mainLoop();
	}

	return 0;
}