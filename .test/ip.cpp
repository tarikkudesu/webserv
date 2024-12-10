#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main()
{
	char *name = strdup("e1r7p7.1337.ma");
	std::cout << name << "\n";

	struct addrinfo hint;
	struct addrinfo *result;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	int status = getaddrinfo(name, NULL, &hint, &result);
	if (status == 0)
	{
		struct addrinfo *tmp = result;
		while (tmp)
		{
			struct sockaddr_in	*add = (struct sockaddr_in *)tmp->ai_addr;
			char	addressString[INET6_ADDRSTRLEN];
			inet_ntop(tmp->ai_family, &(add->sin_addr), addressString, sizeof(addressString));
			std::cout << addressString << "\n";
			tmp = tmp->ai_next;
		}
	}
	else
	{
		std::cerr << "Error" << std::endl;
	}
	freeaddrinfo(result);
}
