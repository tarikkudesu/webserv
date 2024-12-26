#include "WSU.hpp"

wsu::wsu() {}

wsu::wsu(const wsu &copy) { (void)copy; }

wsu &wsu::operator=(const wsu &assign)
{
	(void)assign;
	return *this;
}

wsu::~wsu() {}

bool wsu::__criticalOverLoad = false;

/************************************************************************************************
 *                                             LOGS                                             *
 ************************************************************************************************/
String wsu::logDate()
{
	char buffer[30];
	std::time_t t = std::time(NULL);
	std::tm *tm = std::gmtime(&t);
	std::strftime(buffer, sizeof(buffer), "[%d/%b/%Y:%H:%M:%S]", tm);
	return String(buffer);
}
void wsu::debug(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << MAGENTA << " [DEBUG] " << RESET << __log_message << std::endl;
	return;
}
void wsu::terr(char *__error_message)
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}
void wsu::terr(String __error_message)
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}
void wsu::log(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << RESET << " " << __log_message << std::endl;
	return;
}
void wsu::success(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << GREEN << " [SUCCESS] " << RESET << __log_message << std::endl;
	return;
}
void wsu::running(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << GREEN << " [RUNNING] " << RESET << __log_message << std::endl;
	return;
}
void wsu::warn(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << YELLOW << " [WARN] " << RESET << __log_message << std::endl;
	return;
}
void wsu::error(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << RED << " [ERROR] " << RESET << __log_message << std::endl;
	return;
}
String wsu::buildIMFDate()
{
	char buffer[30];
	std::time_t t = std::time(NULL);
	std::tm *tm = std::gmtime(&t);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return String(buffer);
}
/*************************************************************************************************
 *                                           UTILITIES                                           *
 *************************************************************************************************/
void wsu::trimSpaces(String &str)
{
	if (str.empty())
		return;
	size_t start = 0;
	size_t end = str.length() - 1;
	while (start <= end && std::isspace(str[start]))
		++start;
	while (end >= start && std::isspace(str[end]))
		--end;
	if (start > end)
		str.clear();
	else
		str = str.substr(start, end - start + 1);
}
std::vector<String> wsu::splitBySpaces(const String &input)
{
	String word;
	std::istringstream iss(input);
	std::vector<String> result;
	while (iss >> word)
		result.push_back(word);
	return result;
}
std::vector<String> wsu::splitByChar(const String &input, char del)
{
	std::vector<String> result;
	String temp;
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (input[i] == del)
		{
			if (!temp.empty())
				result.push_back(temp);
			temp.clear();
		}
		else
			temp += input[i];
	}
	if (!temp.empty())
		result.push_back(temp);
	return result;
}
String wsu::intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}
int wsu::stringToInt(const String &str)
{
	std::istringstream iss(str);
	int number = 0;
	iss >> number;
	return number;
}
int wsu::hexToInt(const String &str)
{
	int number = 0;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> number;
	return number;
}
void wsu::replaceString(String &original, const String toReplace, const String replacement)
{
	size_t pos = 0;
	while ((pos = original.find(toReplace, pos)) != String::npos)
	{
		original.replace(pos, toReplace.length(), replacement);
		pos += replacement.length();
	}
}
void wsu::setNonBlockingMode(int sd)
{
	int flags = fcntl(sd, F_GETFL, 0);
	if (flags < 0 || fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl syscall, failed to make a non blocking socket");
}
void wsu::toUpperString(String &input)
{
	for (size_t i = 0; i < input.length(); ++i)
	{
		input[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
	}
}
void wsu::toLowerString(String &input)
{
	for (size_t i = 0; i < input.length(); ++i)
	{
		input[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(input[i])));
	}
}
String wsu::mergeByChar(const std::vector<String> &input, char del)
{
	String result;
	for (std::vector<String>::const_iterator it = input.begin(); it != input.end(); it++)
	{
		result += *it;
		if (it != input.end() - 1)
			result += del;
	}
	return result;
}
bool wsu::samePath(const String &path1, const String &path2)
{
	t_svec vPath1 = wsu::splitByChar(path1, '/');
	t_svec vPath2 = wsu::splitByChar(path2, '/');
	size_t pos = 0;
	if (vPath1.size() != vPath2.size())
		return false;
	for (; pos < vPath1.size() && vPath1.at(pos) == vPath2.at(pos); pos++)
	{
	}
	if (pos == vPath1.size())
		return true;
	return false;
}
bool wsu::containsPath(const String &path, const String &subPath)
{
	t_svec vPath = wsu::splitByChar(path, '/');
	t_svec vSubPath = wsu::splitByChar(subPath, '/');
	size_t pos = 0;
	if (vPath.empty())
		return true;
	if (vPath.size() >= vSubPath.size())
		return false;
	while (pos < vPath.size())
	{
		if (vPath.at(pos) != vSubPath.at(pos))
			return false;
		pos++;
	}
	return true;
}
struct pollfd *wsu::data(t_events &events)
{
	struct pollfd *arr = new struct pollfd[events.size()];
	for (size_t i = 0; i < events.size(); ++i)
		arr[i] = events[i];
	return arr;
}
