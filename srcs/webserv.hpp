#ifndef __WEBSERV_HPP__
#define __WEBSERV_HPP__

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>
#include <ctime>
#include <poll.h>
#include <signal.h>

typedef std::string String;
typedef std::vector<pollfd> t_events;

#define RED "\033[1;31m"
#define BLUE "\033[0;34m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define MAGENTA "\033[1;35m"
#define RESET "\033[1;0m"

#define MAX_EVENTS 1024
#define READ_SIZE 4096

#define PROTOCOLE_V "HTTP/1.1"
#define URI_CHAR_SET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:/?#[]@!$&\'()*+,;=-._~"
#define H_KEY_CHAR_SET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&\'*+-.^_`|~"
#define PRINTABLE " \t\n\r\v\f0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
#define ACCEPT_RANGE_HF__(value) ("Accept-Ranges: " + value)
#define CONNECTION_TYPE_HF__(value) ("Connection: " + value)

typedef enum e_endian
{
	SERVER,
	CONNECTION,
} t_endian;

typedef enum e_method
{
	OPTIONS,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	TRACE,
	CONNECT,
} t_method;

typedef enum e_connectionType
{
	CLOSE,
	KEEP_ALIVE,
} t_connectionType;

typedef enum e_transferEncoding
{
	GENERAL,
	CHUNKED,
} t_transferEncoding;

typedef enum e_uri
{
	DIRECTORY,
	FILE_TOO_SERVE,
	FILE_TO_CREATE,
	CGI_PROGRAM,
} t_URI;

typedef enum e_requestPhase
{
	COMPLETE,
	PROCCESSING,
} t_requestPhase;

#endif
