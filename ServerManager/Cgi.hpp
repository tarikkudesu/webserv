#ifndef __CGI_HPP__
# define __CGI_HPP__

# include "./Request.hpp"
# include "../srcs/webserv.hpp"

# ifndef JAVABIN
#  define JAVABIN "../cgi-bin/java/java"
# endif

# ifndef PYTHONBIN
#  define PYTHONBIN "../cgi-bin/python/python"
# endif

typedef std::map<String, String>::iterator mapIterator ;

class Cgi
{
    String      __body;
    Request     __request;
    String      __ressource;


    /* ______________LOGIC_BUILDING_FUNCTION______________ */
    void        execute(const char* bin, const char *path, int fd);
    const char  *getBin(void);
    void	    cgiProcess(void);
    String      &readFromPipe(int fd);
    bool        endWith(String& file, const char* extention);
    Cgi(); //construction object without pparams is not allowed 

    /*________________ACCESSIBLE FUNCTIONS______________*/
    public:
        ~Cgi();
        Cgi::Cgi(Request __request, String ressource);

        String&     getBody();
};

#endif