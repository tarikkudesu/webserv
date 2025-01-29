#include "WSU.hpp"

wsu::wsu() {}
wsu::wsu(const wsu &copy) { (void)copy; }
wsu &wsu::operator=(const wsu &assign)
{
	(void)assign;
	return *this;
}
wsu::~wsu() {}

std::map<int16_t, String> wsu::__defaultErrorPages;
std::map<int16_t, String> wsu::__errCode;
std::map<String, String> wsu::__mimeTypes;
bool wsu::__criticalOverLoad = false;
bool wsu::__debug = false;
bool wsu::__info = false;
bool wsu::__warn = false;
bool wsu::__error = false;
bool wsu::__fatal = false;
wsu::persist::persist(void) {}
const char *wsu::persist::what(void) const throw() { return "persist"; }

/************************************************************************************************
 *                                             LOGS                                             *
 ************************************************************************************************/

String wsu::generateTimeBasedFileName()
{
	static unsigned long cpt;
	return "./.temp/." + wsu::intToString(std::time(NULL) + cpt++) + ".tmp"; // change path Later
}

bool wsu::endWith(const std::string &file, const char *extension)
{
	int fileLen = file.length();
	int exLen = strlen(extension);

	if (fileLen < exLen)
		return false;

	return file.compare(fileLen - exLen, exLen, extension) == 0;
}

void wsu::logs(std::vector<String> &args)
{
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;
	if (args.size() > 6)
	{
		std::cerr << USAGE << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (args.size() == 0)
		args.push_back("./conf/webserv_default.conf");
	else if (args.size() == 1)
		;
	else if (args.size() == 2)
	{
		if ((args.at(0) != "-l" &&
			 args.at(0) != "--logs") ||
			(args.at(1) != "debug" &&
			 args.at(1) != "info" &&
			 args.at(1) != "warn" &&
			 args.at(1) != "error" &&
			 args.at(1) != "fatal" &&
			 args.at(1) != "all"))
		{
			std::cerr << USAGE << std::endl;
			exit(EXIT_FAILURE);
		}
		args.push_back("./conf/webserv_default.conf");
	}
	else
	{
		if (*(args.end() - 1) == "debug" ||
			*(args.end() - 1) == "info" ||
			*(args.end() - 1) == "warn" ||
			*(args.end() - 1) == "error" ||
			*(args.end() - 1) == "fatal" ||
			*(args.end() - 1) == "all")
		{
			args.push_back("./conf/webserv_default.conf");
		}
	}
	for (std::vector<String>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		if (*it == "debug" && !wsu::__debug)
			wsu::__debug = true;
		else if (*it == "info" && !wsu::__info)
			wsu::__info = true;
		else if (*it == "warn" && !wsu::__warn)
			wsu::__warn = true;
		else if (*it == "error" && !wsu::__error)
			wsu::__error = true;
		else if (*it == "fatal" && !wsu::__fatal)
			wsu::__fatal = true;
		else if (*it == "all")
		{
			wsu::__info = true;
			wsu::__warn = true;
			wsu::__error = true;
			wsu::__fatal = true;
		}
		else if (it == args.end() - 1)
			;
		else if (it == args.begin() && (args.at(0) == "-l" || args.at(0) == "--logs"))
			;
		else
		{
			std::cerr << USAGE << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	try
	{
		if (READ_SIZE < 1024)
			throw std::runtime_error("read size less then 1024 is not recommended");
		wsu::loadErrPages();
		wsu::loadMimeTypes();
		wsu::loadErrorCodes();
	}
	catch (std::exception &e)
	{
		wsu::terr(e.what());
		exit(EXIT_FAILURE);
	}
}
void wsu::debug(String __log_message)
{
	if (wsu::__debug)
		std::cout << BLUE << wsu::logDate() << MAGENTA << " [DEBUG] " << RESET << __log_message << std::endl;
}
void wsu::info(String __log_message)
{
	if (wsu::__info)
		std::cout << BLUE << wsu::logDate() << GREEN << " [INFO] " << RESET << __log_message << std::endl;
}
void wsu::warn(String __log_message)
{
	if (wsu::__warn)
		std::cout << BLUE << wsu::logDate() << YELLOW << " [WARN] " << RESET << __log_message << std::endl;
}
void wsu::error(String __log_message)
{
	if (wsu::__error)
		std::cout << BLUE << wsu::logDate() << RED << " [ERROR] " << RESET << __log_message << std::endl;
}
void wsu::fatal(String __log_message)
{
	if (wsu::__fatal)
		std::cout << BLUE << wsu::logDate() << RED << " [FATAL] " << RESET << __log_message << std::endl;
}
void wsu::running(String __log_message)
{
	std::cout << BLUE << wsu::logDate() << GREEN << " [RUNNING] " << RESET << __log_message << std::endl;
}
void wsu::terr(char *__error_message)
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}
void wsu::terr(String __error_message)
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}
/*************************************************************************************************
 *                                           UTILITIES                                           *
 *************************************************************************************************/
String wsu::generateTokenId()
{
    String tokenId;
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string specialChars = "123456789";
	static int sed = 0;
	for (int i = 0; i < 10; i++)
	{
		std::srand(static_cast<unsigned int>(std::time(0) + sed++));
		int randomAlphabetIndex = std::rand() % alphabet.size();
		int randomSpecialIndex = std::rand() % specialChars.size();
		tokenId += alphabet[randomAlphabetIndex];
		tokenId += specialChars[randomSpecialIndex];
	}
    return tokenId;
}
String wsu::logDate()
{
	char buffer[30];
	std::time_t t = std::time(NULL);
	std::tm *tm = std::gmtime(&t);
	std::strftime(buffer, sizeof(buffer), "[%d/%b/%Y:%H:%M:%S]", tm);
	return String(buffer);
}
String wsu::buildIMFDate()
{
	char buffer[30];
	std::time_t t = std::time(NULL);
	std::tm *tm = std::gmtime(&t);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return String(buffer);
}
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
String wsu::getParentPath(const String &path)
{
	if (path.empty())
		return "";
	t_svec vPath = wsu::splitByChar(path, '/');
	if (!vPath.empty())
		return *(vPath.end() - 1);
	return "";
}
String wsu::joinPaths(const String &path1, const String &path2)
{
	if (path1.empty() && path2.empty())
		return "./";
	else if (path2.empty())
		return path1;
	else if (path1.empty())
		return path2;
	if (*(path1.end() - 1) != '/' && *(path2.begin()) != '/')
		return path1 + "/" + path2;
	if (*(path1.end() - 1) == '/' && *(path2.begin()) == '/')
		return path1 + path2.substr(1, path2.length());
	return path1 + path2;
}
String wsu::resolvePath(const String &path)
{
	t_svec vPath = wsu::splitByChar(path, '/');
	t_svec newPath;
	for (t_svec::iterator it = vPath.begin(); it != vPath.end(); it++)
	{
		if (*it == ".." && !newPath.empty())
			newPath.pop_back();
		else
			newPath.push_back(*it);
	}
	if (newPath.empty())
		return "./";
	return wsu::mergeByChar(newPath, '/');
}
struct pollfd *wsu::data(t_events &events)
{
	struct pollfd *arr = new struct pollfd[events.size()];
	for (size_t i = 0; i < events.size(); ++i)
		arr[i] = events[i];
	return arr;
}
String wsu::getContentType(const String &uri)
{
	t_svec tmp = wsu::splitByChar(uri, '/');
	if (tmp.empty())
		return "text/html";
	String file = *(tmp.end() - 1);
	size_t dot_pos = file.rfind('.');
	if (dot_pos == String::npos)
		return "text/html";
	String ext = file.substr(dot_pos + 1);
	std::map<String, String>::iterator it = wsu::__mimeTypes.find(ext);
	if (it != __mimeTypes.end())
		return it->second;
	return "text/html";
}

void wsu::loadErrorCodes()
{
	wsu::info("error codes");
	if (!wsu::__errCode.empty())
		return;
	std::fstream fs;
	String line;
	fs.open("essentials/errorcodes.csv");
	if (!fs.is_open())
		throw std::runtime_error("could not open: essentials/errorcodes.csv");
	std::stringstream stream;
	do
	{
		std::getline(fs, line, '\n');
		if (fs.eof())
			break;
		if (fs.fail())
		{
			fs.close();
			throw std::runtime_error("could not read from essentials/errorcodes.csv");
		}
		t_svec vect = wsu::splitByChar(line, ',');
		if (vect.size() == 2)
		{
			wsu::trimSpaces(vect[0]), wsu::trimSpaces(vect[1]);
			wsu::__errCode.insert(std::make_pair(wsu::stringToInt(vect[0]), vect[1]));
		}
		line.clear();
	} while (true);
	fs.close();
}
void wsu::loadErrPages()
{
	wsu::info("default error pages");
	if (!wsu::__defaultErrorPages.empty())
		return;
	std::fstream fs;
	String line;
	fs.open("essentials/errorpages.csv");
	if (!fs.is_open())
		throw std::runtime_error("could not open: essentials/errorpages.csv");
	std::stringstream stream;
	do
	{
		std::getline(fs, line, '\n');
		if (fs.eof())
			break;
		if (fs.fail())
		{
			fs.close();
			throw std::runtime_error("could not read from essentials/errorpages.csv");
		}
		t_svec vect = wsu::splitByChar(line, ',');
		if (vect.size() == 2)
		{
			wsu::trimSpaces(vect[0]), wsu::trimSpaces(vect[1]);
			wsu::__defaultErrorPages.insert(std::make_pair(wsu::stringToInt(vect[0]), vect[1]));
		}
		line.clear();
	} while (true);
	fs.close();
}
void wsu::loadMimeTypes(void)
{
	wsu::info("mime types");
	if (!wsu::__mimeTypes.empty())
		return;
	std::ifstream fs;
	String line;
	fs.open("essentials/mimetypes.csv");
	if (!fs.is_open())
		throw std::runtime_error("could not open: essentials/mimetypes.csv");
	std::stringstream stream;
	do
	{
		std::getline(fs, line, '\n');
		if (fs.eof())
			break;
		if (fs.fail())
		{
			fs.close();
			throw std::runtime_error("could not read from essentials/mimetypes.csv");
		}
		t_svec vect = wsu::splitByChar(line, ',');
		if (vect.size() == 2)
		{
			wsu::trimSpaces(vect[0]), wsu::trimSpaces(vect[1]);
			wsu::__mimeTypes.insert(std::make_pair(vect[0], vect[1]));
		}
		line.clear();
	} while (true);
	fs.close();
}
String wsu::buildListingBody(String path, const t_svec &list)
{
	String body = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">"
				  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title>"
				  "<style>a {display: block;font-family: sans-serif;color: rgb(184, 186, 190);text-decoration: none;letter-spacing: 0.8px;padding: 5px 10px;font-weight: 500;margin: 4px;}"
				  "a:hover {border-radius: 2px;color: white;background-color: rgb(35, 40, 47);}"
				  ".path {color: white;font-size: 25px;font-weight: 700;padding: 5px 10px;font-family: sans-serif;margin-bottom: 10px;border-bottom: 1px solid rgba(210, 215, 223, 0.26);}"
				  "</style></head><body style=\"background-color: rgb(35, 40, 47);\">"
				  "<div style=\"border: 1px solid rgba(210, 215, 223, 0.26); border-radius: 4px; margin: 80px; padding: 40px; background-color: rgb(22, 27, 34);\">"
				  "<div class=\"path\">PATH</div>LISTING</div></body></html>";
	String anchor = "<a href=\"LINK\">NAME</a>";
	std::stringstream ss;
	String listings;
	for (t_svec::const_iterator it = list.begin(); it != list.end(); it++)
	{
		if (it->empty() || String::npos == it->find_first_not_of(" \t\n\r\v\f") || *it == "." || *it == "..")
			continue;
		String listing = anchor;
		String link = wsu::joinPaths(wsu::getParentPath(path), *it);
		wsu::replaceString(listing, "LINK", link);
		wsu::replaceString(listing, "NAME", *it);
		ss << listing;
	}
	wsu::replaceString(body, "PATH", path);
	wsu::replaceString(body, "LISTING", ss.str());
	return body;
}
