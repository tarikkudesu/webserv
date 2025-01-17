#ifndef __CGI_HPP__
#define __CGI_HPP__

# include "./Get.hpp"

class Cgi
{
	private:
		Request&                __request;
		RessourceHandler&       __explorer;
		Location&               __location;
		std::clock_t            __start;
		String                  __body;
		char					**env;


		void                    execute(const char *bin, const char *path, int fd);
		const char*             getBin(void);
		void                    readFromPipe(int fd);
		void                    setCgiEnvironement();
		void                    cgiProcess(void);
		void					clear( void );		
		const					char *getMethod();
		String					getQueryString();//tmp function



	public:
		String&                  getBody();

		Cgi(RessourceHandler &explorer, Request &request, Location &location);
		~Cgi();
		// Cgi(const Cgi &copy);
		// Cgi &operator=(const Cgi &assign);
};

#endif