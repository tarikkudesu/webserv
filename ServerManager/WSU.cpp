#include "WSU.hpp"

WSU::WSU() {}

WSU::WSU(const WSU &copy) { (void)copy; }

WSU::~WSU() {}

WSU &WSU::operator=(const WSU &assign)
{
	(void)assign;
	return *this;
}

bool WSU::__criticalOverLoad = false;
int WSU::__memory = 0;
int WSU::__l = 0;
int WSU::__s = 0;
int WSU::__c = 0;

std::string WSU::logDate()
{
	char buffer[30];
	std::time_t t = std::time(nullptr); // Get the current time
	std::tm *tm = std::gmtime(&t);		// Convert time to GMT (UTC)
	// Format the date in IMF format: Day, DD Mon YYYY HH:MM:SS GMT
	std::strftime(buffer, sizeof(buffer), "[%d/%b/%Y:%H:%M:%S]", tm);
	return std::string(buffer); // Return the date as a string
}
std::string WSU::buildIMFDate()
{
	char buffer[30];
	std::time_t t = std::time(nullptr); // Get the current time
	std::tm *tm = std::gmtime(&t);		// Convert time to GMT (UTC)
	// Format the date in IMF format: Day, DD Mon YYYY HH:MM:SS GMT
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer); // Return the date as a string
}
/************************************************************************************************
 *                                             LOGS                                             *
 ************************************************************************************************/
void WSU::debug(String __log_message)
{
	if (__log_message.find("allocated") != String::npos)
	{
		if (__log_message.find("location") != String::npos)
			WSU::__l++;
		if (__log_message.find("server") != String::npos)
			WSU::__s++;
		if (__log_message.find("connection") != String::npos)
			WSU::__c++;
	}
	else if (__log_message.find("freed") != String::npos)
	{
		if (__log_message.find("location") != String::npos)
			WSU::__l--;
		if (__log_message.find("server") != String::npos)
			WSU::__s--;
		if (__log_message.find("connection") != String::npos)
			WSU::__c--;
	}
	std::cout << BLUE << WSU::logDate() << MAGENTA << " [DEBUG] " << RESET << __log_message << std::endl;
	return;
}
void WSU::terr(char *__error_message)
{
	std::cerr << BLUE << WSU::logDate() << RED << " [error] " << RESET << __error_message << std::endl;
}
void WSU::terr(String __error_message)
{
	std::cerr << BLUE << WSU::logDate() << RED << " [error] " << RESET << __error_message << std::endl;
}
void WSU::log(String __log_message)
{
	return;
	std::cout << BLUE << WSU::logDate() << RESET << " " << __log_message << std::endl;
}
void WSU::success(String __log_message)
{
	return;
	std::cout << BLUE << WSU::logDate() << GREEN << " [SUCCESS] " << RESET << __log_message << std::endl;
}
void WSU::running(String __log_message)
{
	return;
	std::cout << BLUE << WSU::logDate() << GREEN << " [RUNNING] " << RESET << __log_message << std::endl;
}
void WSU::warn(String __log_message)
{
	return;
	std::cout << BLUE << WSU::logDate() << YELLOW << " [WARN] " << RESET << __log_message << std::endl;
}
void WSU::error(String __log_message)
{
	return;
	std::cout << BLUE << WSU::logDate() << RED << " [ERROR] " << RESET << __log_message << std::endl;
}
/*************************************************************************************************
 *                                           UTILITIES                                           *
 *************************************************************************************************/
void WSU::trimSpaces(String &str)
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
std::vector<std::string> WSU::splitBySpaces(const std::string &input)
{
	std::string word;
	std::istringstream iss(input);
	std::vector<std::string> result;
	while (iss >> word)
		result.push_back(word);
	return result;
}
std::string WSU::intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}
int WSU::stringToInt(const std::string &str)
{
	std::istringstream iss(str);
	int number = 0;
	iss >> number;
	return number;
}
int WSU::hexToInt(const std::string &str)
{
	int number = 0;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> number;
	return number;
}
void WSU::replaceString(String &original, const String toReplace, const String replacement)
{
	size_t pos = 0;
	while ((pos = original.find(toReplace, pos)) != String::npos)
	{
		original.replace(pos, toReplace.length(), replacement);
		pos += replacement.length();
	}
}
void WSU::setNonBlockingMode(int sd)
{
	int flags = fcntl(sd, F_GETFL, 0);
	if (flags < 0 || fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl syscall, failed to make a non blocking socket");
}
void WSU::toUpperString(std::string &input)
{
	for (size_t i = 0; i < input.length(); ++i)
	{
		input[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
	}
}
void WSU::toLowerString(std::string &input)
{
	for (size_t i = 0; i < input.length(); ++i)
	{
		input[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(input[i])));
	}
}
