#ifndef __CGI_HPP__
#define __CGI_HPP__

#include "./Request.hpp"

class Cgi
{
	private:
                Request&                request;
                String                  body;
                RessourceHandler&       explorer;

        public:
                String&                  getBody();

                Cgi(RessourceHandler& explorer, Request& request);
		Cgi(const Cgi &copy);
		Cgi &operator=(const Cgi &assign);
		~Cgi();
};

#endif