#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "Location.hpp"

class Client
{
	private:
		std::map<String, String>	headers;
		std::map<String, String>	queryVars;
		int 						fd;
		String						buffer;
		short						status;
		String						path;
		String						requestBody;

	public:
		Client(void);
		Client( String requestLine, String requestHeaders );
		Client(int fd);
		~Client();
		Client(const Client& client);
		Client& operator=(const Client& client);

		void						appendBuffer(String buffer);
		void						erase(int pos);
		void						setBody(String body);
		String						getBody(void);
		void						setStatus(short status);
		short						getStatus(void);
		String						getBuffer(void);
		void						setBuffer(String buffer);
		String						getPath(void);
		void						setPath(String path);
		String						getHeader(String key);
		void						setHeader(String key, String value);
		std::map<String, String>	getQueryVars(void);
		void						setQueryVars(std::map<String, String>& query);
};

#endif