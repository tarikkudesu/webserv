#ifndef __WSU_HPP__
#define __WSU_HPP__

#include "../srcs/webserv.hpp"

class wsu
{
private:
	wsu();
	wsu(const wsu &copy);
	wsu &operator=(const wsu &assign);
	~wsu();

public:
	static bool __criticalOverLoad;
	static bool	__debug;
	static bool	__info;
	static bool	__warn;
	static bool	__error;
	static bool	__fatal;

	static void	logs(std::vector<std::string> &args);
	static void debug(String __log_message);
	static void info(String __log_message);
	static void warn(String __log_message);
	static void error(String __log_message);
	static void fatal(String __log_message);
	static void running(String __log_message);
	static void terr(String __error_message);
	static void terr(char *__error_message);

	static String logDate();
	static String buildIMFDate();
	static void trimSpaces(String &str);
	static String intToString(int number);
	static int hexToInt(const String &str);
	static void setNonBlockingMode(int sd);
	static void toUpperString(String &input);
	static void toLowerString(String &input);
	static int stringToInt(const String &str);
	static struct pollfd *data(t_events &events);
	static std::vector<String> splitBySpaces(const String &input);
	static bool samePath(const String &path1, const String &path2);
	static bool containsPath(const String &path, const String &subPath);
	static String mergeByChar(const std::vector<String> &input, char del);
	static std::vector<String> splitByChar(const String &input, char del);
	static void replaceString(String &original, const String toReplace, const String replacement);
};

#endif
