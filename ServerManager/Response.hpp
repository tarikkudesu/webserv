#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include "Cgi.hpp"

class Response
{
	private:
		Server&						__server;
		Request&					__request;
		Location&					__location;
		t_svec						autoIndex;
	
		/*	Response components */
		int							code;
		String						ressource;
		String						codeMessage;
		String						content_type;
		size_t						content_length;
		String						protocole;
		std::map<String, String>	header;
		String						body;
	
		/*	flags  */
		bool						isCgi;
		bool						isDir;
		bool						isDone;
		bool						isFound;
		bool						isAutoIndex;
	
		/*	helper methods */
		void						setCode(int code);
		void						setCodeMessage(String message);
		bool						fileFound();
		bool						dirFound();
		bool						isReadable(const char *str);
	
		/*	Method handlers */
		void						handleGET();
		void						handlePOST();
		void						handleDELETE();
		void						executeCgi();
	
		/*	Response building helper */
		void						buildResponseHeaders();
		void						buildResponseBody();
		void						buildResponse();
		String						buildStartLine(String protocle, int code, String codeMessage);
	
		/*	File operations */
		String						readFile(const String &path);
		bool						writeFile(const String &path, const String &content);
		bool						deleteFile(const String &path);
	
		/*	Validation */
		// bool						validateRequest();
		bool						validateMethod();
		bool						isValidCgi();
	
		/*	Resource path resolution	*/
		// String						combinePaths(const String &base, const String &relative);
	
	public:
		t_svec						listDir(String &dirPath);
		String						getResponse();
		String						getStartLine();
		String						getResponseBody();
		const						std::map<String, String> &getHeaders();

		Response(Request &request, Server &server, Location &location);
		Response(const Response &copy);
		Response &operator=(const Response &assign);
		~Response();
};

#endif
