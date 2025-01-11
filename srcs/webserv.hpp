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
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>
#include <ctime>
#include <poll.h>
#include <signal.h>
#include <netdb.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <cstdlib>

#ifndef TIMEOUT
#define TIMEOUT 5000000
#endif

#ifndef JAVABIN
#define JAVABIN "/usr/bin/java"
#endif

#ifndef PYTHONBIN
#define PYTHONBIN "/usr/bin/php"
#endif

typedef std::string String;
typedef std::vector<pollfd> t_events;
typedef std::vector<String> t_svec;
typedef std::map<String, String>::iterator mapIterator;

#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
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
#define USAGE "\nUsage: ./webserv [OPTIONS] [configuration file]\n\nOptions:\n\t-l, --logs string\tlog events(\"debug\"|\"info\"|\"warn\"|\"error\"|\"fatal\"|\"all\")\n"

typedef enum e_multipartsection
{
	MP_HEADERS,
	MP_BODY,
} t_multipartsection;

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

typedef enum e_type
{
	FILE_,
	FOLDER
} t_type;

typedef enum e_connectionType
{
	CLOSE,
	KEEP_ALIVE,
} t_connectionType;

typedef enum e_bodyType
{
	NONE,
	DEFINED,
	CHUNKED,
	MULTIPART,
} t_bodyType;

typedef enum e_uri
{
	DIRECTORY,
	FILE_TOO_SERVE,
	FILE_TO_CREATE,
	CGI_PROGRAM,
} t_URI;

typedef enum e_requestPhase
{
	NEWREQUEST,
	INITIALIZING,
	PROCESSING,
	COMPLETE,
} t_requestPhase;

#endif
