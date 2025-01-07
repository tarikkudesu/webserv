#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

# include "RessourceHandler.hpp"
# include "Cgi.hpp"
# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"

typedef enum
{
        FILE,
        FOLDER
} Type;

class Response
{
	private:
		RessourceHandler				explorer;
		Server&						__server;
		Request&					__request;
		Location&					__location;
		int						code;
		String						reasonPhrase;
		std::map<String, String>			headers;
		String						body;
		String						fullResponse;
		void						__check_methods();
		void						buildResponse();
		void						setHeader();
		void						executeGet( void );
		void						executePost( void );
		void						executeDelete( void );
		void						executeCgi( void );


	public:

		String	getResponse( void );

		Response(Request &request, Server &server, Location &location);
		Response(const Response &copy);
		Response &operator=(const Response &assign);
		~Response();
};

#endif
