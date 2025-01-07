#ifndef __CGI_HPP__
#define __CGI_HPP__

#include "./Request.hpp"

class Cgi
{
	std::clock_t		start;
	String				__body;
	String				respose;
	Request&			__request;
	Location&			__location;
	String				__ressource;

	/* ______________LOGIC_BUILDING_FUNCTIONS______________ */
	void				execute(const char *bin, const char *path, int fd);
	void				setCgiEnvironement();
	const char			*getBin(void);
	void				cgiProcess(void);
	void				readFromPipe(int fd);

	/*________________ACCESSIBLE FUNCTIONS______________*/

public:
	Cgi(Request &__request, Location &location, String ressource);
	~Cgi();
	String &getBody();
};

#endif