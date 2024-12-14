#include "Config.hpp"

Config::Config(String configuration_file) : __configFile(configuration_file)
{
	String line;
	__fS.open(configuration_file);
	if (!__fS.is_open())
		throw std::runtime_error("coudln't open file");
	do
	{
		std::getline(this->__fS, line, '\n');
		if (this->__fS.fail())
			break;
		if (line.empty() || String::npos == line.find_first_not_of(" \t\n\r\v\f"))
			continue;
		this->__lines.append(line);
		this->__lines.append(" ");
		line.clear();
		if (this->__fS.eof())
			break;
	} while (true);
	__fS.close();
}

Config::Config(const Config &copy)
{
	*this = copy;
}

Config::~Config()
{
}

Config &Config::operator=(const Config &assign)
{
	if (this != &assign)
	{
		this->__configFile = assign.__configFile;
	}
	return *this;
}
void Config::firstCheck()
{
	if (__lines.empty())
		throw std::runtime_error("empty file");
	if (String::npos == __lines.find_first_of("{}"))
		throw std::runtime_error("invalid config file 1");
	if (String::npos != __lines.find_first_not_of(PRINTABLE))
		throw std::runtime_error("unknown characters");
}

void Config::reduceSpaces()
{
	std::string result;
	bool inSpace = false;

	for (size_t i = 0; i < __lines.length(); i++)
	{
		if (std::isspace(__lines.at(i)))
		{
			if (!inSpace)
			{
				result += ' ';
				inSpace = true;
			}
		}
		else
		{
			result += __lines.at(i);
			inSpace = false;
		}
	}
	this->__lines.clear();
	this->__lines.append(result);
}
void Config::checkBraces()
{
	size_t end = 0;
	size_t tracker = 0;
	do
	{
		if (end >= this->__lines.length())
			break;
		if (this->__lines.at(end) == '}')
			tracker--;
		if (this->__lines.at(end) == '{')
			tracker++;
		end++;
	} while (true);
	if (tracker != 0)
		throw std::runtime_error("unclosed curly braces");
}
void Config::checkOuterscope(String outerScope)
{
	WSU::trimSpaces(outerScope);
	if (outerScope != "server")
		throw std::runtime_error("invalid config file 2");
	if (__lines.find_first_of("{}") == String::npos)
		throw std::runtime_error("invalid config file 3");
}
void Config::setUpServer(size_t start)
{
	size_t end = start + 1;
	size_t tracker = 1;

	checkOuterscope(String(this->__lines.begin(), this->__lines.begin() + start));
	do
	{
		if (end >= this->__lines.length())
			break;
		if (this->__lines.at(end) == '}')
			tracker--;
		if (this->__lines.at(end) == '{')
			tracker++;
		end++;
		if (tracker == 0)
			break;
	} while (true);
	if (tracker != 0)
		throw std::runtime_error("unclosed curly braces");
	String serverConfig(this->__lines.begin() + start, this->__lines.begin() + end);
	this->__lines.erase(0, end);
	Template *temp = new Template(serverConfig);
}
void Config::setUpServers()
{
	do
	{
		size_t pos = this->__lines.find("{");
		if (pos == String::npos && __lines.find_first_not_of(" \t\n\r\v\f") != String::npos)
			throw std::runtime_error("invalid config file 4");
		else if (pos == String::npos)
			break;
		setUpServer(pos);
	} while (!this->__lines.empty());
}

void Config::setupEverything()
{
	firstCheck();
	reduceSpaces();
	checkBraces();
	setUpServers();
}
