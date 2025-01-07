#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

# include "Cgi.hpp"
# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"
# include "RessourceHandler.hpp"

typedef enum
{
        FILE,
        DIR
} Type;

class Response
{
	private:
		Server&						__server;
		Request&					__request;
		Location&					__location;
	
	public:

		Response(Request &request, Server &server, Location &location);
		Response(const Response &copy);
		Response &operator=(const Response &assign);
		~Response();
};

#endif
