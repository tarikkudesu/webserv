
# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <string.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <iostream>
# include <vector>
# include <map>
# include <iomanip>
# include <algorithm>

# define SERVER_PORT	443
# define MAXLINE		4096
# define SA				struct sockaddr

int main( int ac, char **av ) {
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	if (ac != 2) {
		std::cerr << "Wrong arguments\n";
		return 1;
	}
	
	int 				sockfd, n;
	struct sockaddr_in	servaddr;
	int					sendbytes;
	char				sendline[MAXLINE];
	char				recvline[MAXLINE];

	// AF_INET == internet address family
	// SOCK_STREAM == stream socket
	// 0 == TCP protocole
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "socket syscall error\n";
		perror("err: ");
		return 1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) < 0) {
		std::cerr << "inet_pton syscall err\n";
		perror("err: ");
		return 1;
	}
	
	if (connect(sockfd, (SA *)&servaddr, sizeof(struct sockaddr_in)) < 0) {
		std::cerr << "connect syscall err\n";
		perror("err: ");
		return 1;
	}

	sprintf(sendline, "GET / HTTP/1.1\r\nHost: %s:443\r\n\r\n", av[1]);
	sendbytes = strlen(sendline);

	if (write(sockfd, sendline, sendbytes) != sendbytes) {
		std::cerr << "write syscall err\n";
		perror("err: ");
		return 1;
	}
	
	memset(recvline, 0, MAXLINE);

	while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0)
	{
		recvline[n] = 0;
		printf("%s\n", recvline);
	}
	if (n < 0) {
		std::cerr << "read syscall err\n";
		perror("err: ");
		return 1;
	}

	exit (0);
}
