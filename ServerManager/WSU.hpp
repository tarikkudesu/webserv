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
	class persist : public std::exception {
		public :
			persist();
			virtual const char	*what( void ) const throw();
	};

	static std::map<int16_t, String> __defaultErrorPages;
	static std::map<int16_t, String> __errCode;
	static std::map<String, String> __mimeTypes;
	static bool __criticalOverLoad;
	static bool __debug;
	static bool __info;
	static bool __warn;
	static bool __error;
	static bool __fatal;

	static void logs(std::vector<std::string> &args);
	static void debug(String __log_message);
	static void info(String __log_message);
	static void warn(String __log_message);
	static void error(String __log_message);
	static void fatal(String __log_message);
	static void running(String __log_message);
	static void terr(String __error_message);
	static void terr(char *__eror_message);

	static String logDate();
	static void loadErrPages();
	static void loadMimeTypes();
	static void loadErrorCodes();
	static String buildIMFDate();
	static void trimSpaces(String &str);
	static String intToString(int number);
	static int hexToInt(const String &str);
	static void setNonBlockingMode(int sd);
	static void toUpperString(String &input);
	static void toLowerString(String &input);
	static int stringToInt(const String &str);
	static struct pollfd *data(t_events &events);
	static String resolvePath(const String &path);
	static String getParentPath(const String &path);
	static String getContentType(const String &uri);
	static std::vector<String> splitBySpaces(const String &input);
	static bool samePath(const String &path1, const String &path2);
	static String buildListingBody(String path, const t_svec &list);
	static String joinPaths(const String &path1, const String &path2);
	static bool endWith(const std::string &file, const char *extension);
	static bool containsPath(const String &path, const String &subPath);
	static String mergeByChar(const std::vector<String> &input, char del);
	static std::vector<String> splitByChar(const String &input, char del);
	static void replaceString(String &original, const String toReplace, const String replacement);
};

#endif
