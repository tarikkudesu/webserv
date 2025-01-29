#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include "Cgi.hpp"

class Response
{
	private:
		RessourceHandler			explorer;
		Server &					__server;
		Request &					__request;
		Location &					__location;
		String						reasonPhrase;
		std::map<String, String>	headers;
		int 						code;
		std::vector<BasicString>	body;
		Token						token;

		void __check_methods();
		void buildResponse();
		void setHeader();
		void executeGet(void);
		void executePost(void);
		void executeDelete(void);
		void executeCgi(void);
		bool checkCgi();
		void executeAuth();
		bool authenticated();
		bool shouldAuthenticate();


	public:
		const std::vector<BasicString> &getResponse(void) const;
		void print() const;

		Response(Request &request, Server &server, Location &location);
		Response(const Response &copy);
		Response &operator=(const Response &assign);
		~Response();
};

std::ostream &operator<<(std::ostream &o, const Response &r);

#endif
