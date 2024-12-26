
#include "../ServerManager/ServerManager.hpp"

ServerManager *webservP = NULL;

void f()
{
	int fd = open(".logs/sds.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (-1 == fd)
	{
		wsu::terr(String(" .logs/sds.log cannot be opened"));
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	system("lsof -c webserv");
	std::cout << "\n********************************************************************\n";
	std::cout << "********************************************************************\n\n";
	system("leaks -list webserv");
}

void signalHandler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "exiting\n";
		ServerManager::up = false;
	}
}

void buildPage()
{
	String __errPage = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">\n"
					   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
					   "<title>Webserv</title></head><body style=\"background-color: rgb(35, 40, 47);\">\n"
					   "<div style=\"border: 1px solid rgba(210, 215, 223, 0.26); border-radius: 4px; margin-top: 100px; background-color: rgb(22, 27, 34);\">\n"
					   "<h1 style=\"font-size: 80px; font-family: sans-serif; text-align: center; padding: 20px 0px 0px 0px; margin: 0px; color: rgb(210, 215, 223);\">\n"
					   "CODE\n"
					   "</h1><h2 style=\"font-size: 25px; font-family: sans-serif; text-align: center; padding: 0px 0px 10px 0px; margin: 0px; color: rgb(210, 215, 223);\">\n"
					   "REASON_PHRASE\n"
					   "</h2><p style=\"font-size: 16px; font-family: sans-serif; text-align: center; padding: 0px 0px 30px 0px; margin: 0px; color: rgb(90, 139, 223);\">\n"
					   "MESSAGE\n"
					   "</p></div></body></html>";
	std::map<int16_t, String> __errCode;
	__errCode.insert(std::make_pair(100, "Continue"));
	__errCode.insert(std::make_pair(101, "Switching Protocols"));
	__errCode.insert(std::make_pair(200, "OK"));
	__errCode.insert(std::make_pair(201, "Created"));
	__errCode.insert(std::make_pair(202, "Accepted"));
	__errCode.insert(std::make_pair(203, "Non-Authoritative Information"));
	__errCode.insert(std::make_pair(204, "No Content"));
	__errCode.insert(std::make_pair(205, "Reset Content"));
	__errCode.insert(std::make_pair(206, "Partial Content"));
	__errCode.insert(std::make_pair(300, "Multiple Choices"));
	__errCode.insert(std::make_pair(301, "Moved Permanently"));
	__errCode.insert(std::make_pair(302, "Found"));
	__errCode.insert(std::make_pair(303, "See Other"));
	__errCode.insert(std::make_pair(304, "Not Modified"));
	__errCode.insert(std::make_pair(305, "Use Proxy"));
	__errCode.insert(std::make_pair(307, "Temporary Redirect"));
	__errCode.insert(std::make_pair(400, "Bad Request"));
	__errCode.insert(std::make_pair(401, "Unauthorized"));
	__errCode.insert(std::make_pair(402, "Payment Required"));
	__errCode.insert(std::make_pair(403, "Forbidden"));
	__errCode.insert(std::make_pair(404, "Not Found"));
	__errCode.insert(std::make_pair(405, "Method Not Allowed"));
	__errCode.insert(std::make_pair(406, "Not Acceptable"));
	__errCode.insert(std::make_pair(407, "Proxy Authentication Required"));
	__errCode.insert(std::make_pair(408, "Request Time-out"));
	__errCode.insert(std::make_pair(409, "Conflict"));
	__errCode.insert(std::make_pair(410, "Gone"));
	__errCode.insert(std::make_pair(411, "Length Required"));
	__errCode.insert(std::make_pair(412, "Precondition Failed"));
	__errCode.insert(std::make_pair(413, "Request Entity Too Large"));
	__errCode.insert(std::make_pair(414, "Request-URI Too Large"));
	__errCode.insert(std::make_pair(415, "Unsupported Media Type"));
	__errCode.insert(std::make_pair(416, "Requested range not satisfiable"));
	__errCode.insert(std::make_pair(417, "Expectation Failed"));
	__errCode.insert(std::make_pair(500, "Internal Server Error"));
	__errCode.insert(std::make_pair(501, "Not Implemented"));
	__errCode.insert(std::make_pair(502, "Bad Gateway"));
	__errCode.insert(std::make_pair(503, "Service Unavailable"));
	__errCode.insert(std::make_pair(504, "Gateway Time-out"));
	__errCode.insert(std::make_pair(505, "HTTP Version not supported"));
	for (int16_t i = 100; i < 506; i++)
	{
		if (__errCode.find(i) != __errCode.end())
		{
			String	path("./Content/" + wsu::intToString(i) + ".html");
			std::ofstream file(path.c_str());
			if (!file)
			{
				std::cerr << "Error: Could not create or open the file.\n";
				return;
			}
			String page = __errPage;
			wsu::replaceString(page, "CODE", wsu::intToString(i));
			wsu::replaceString(page, "REASON_PHRASE", __errCode[i]);
			wsu::replaceString(page, "MESSAGE", "");
			file << page;
			file.close();
		}
	}
	exit(1);
}

int main(int ac, char **av)
{
	atexit(f);
	signal(SIGINT, signalHandler);
	// signal(SIGPIPE, signalHandler);
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;
	String config;
	if (ac == 2)
		config = *(av + 1);
	else if (ac == 1)
		config = "conf/webserv_default.conf";
	else
	{
		wsu::terr(String("Usage: ./webserv file_path"));
		return 1;
	}
	ServerManager webserv(config);
	webserv.setUpWebserv();
	webserv.clear();
	return 0;
}
