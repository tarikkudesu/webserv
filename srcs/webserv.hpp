#ifndef __WEBSERV__HPP__
#define __WEBSERV__HPP__

# include <iostream>
# include <algorithm>
# include <fstream>
# include <cstring>
# include <string>
# include <vector>
# include <map>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <ctype.h>
# include <time.h>
# include <poll.h>

typedef std::string String;

# define RED            "\033[1;31m"
# define BLUE           "\033[1;34m"
# define CYAN           "\033[1;36m"
# define GREEN          "\033[1;32m"
# define YELLOW         "\033[1;33m"
# define MGENTA         "\033[1;35m"
# define RESET          "\033[1;0m"

# define MAX_EVENTS     1024
# define READ_SIZE      1024

#endif