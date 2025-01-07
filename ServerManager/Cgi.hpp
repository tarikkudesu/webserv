#ifndef __CGI_HPP__
# define __CGI_HPP__

# include "./Request.hpp"
# include "../srcs/webserv.hpp"

# ifndef TIMEOUT
# define TIMEOUT 10000000
#endif

# ifndef JAVABIN
#  define JAVABIN "/usr/bin/java"
# endif

# ifndef PYTHONBIN
#  define PYTHONBIN "/usr/bin/php"
# endif

typedef std::map<String, String>::iterator mapIterator ;

class Cgi
{
    std::clock_t    start;
    String          __body;
    String          respose;
    Request         __request;
    Location&       __location;
    String          __ressource;

    /* ______________LOGIC_BUILDING_FUNCTIONS______________ */
    void        execute(const char* bin, const char *path, int fd);
    void	    setCgiEnvironement();
    const char  *getBin(void);
    void	    cgiProcess(void);
    void        readFromPipe(int fd);

    /*________________ACCESSIBLE FUNCTIONS______________*/

    public:
        ~Cgi();
        Cgi::Cgi(Request __request,Location& location, String ressource);
        String&     getBody();
};

#endif