#include "WSU.hpp"

WSU::WSU() {}

WSU::WSU( const WSU &copy ) { (void) copy; }

WSU::~WSU() {}

WSU	&WSU::operator=( const WSU &assign )
{
	(void) assign;
	return *this;
}

std::string WSU::logDate() {
	char		buffer[30];
	std::time_t	t = std::time(nullptr); // Get the current time
	std::tm		*tm = std::gmtime(&t);	// Convert time to GMT (UTC)
	// Format the date in IMF format: Day, DD Mon YYYY HH:MM:SS GMT
	std::strftime(buffer, sizeof(buffer), "[%d/%b/%Y:%H:%M:%S]", tm);
	return std::string(buffer); // Return the date as a string
}

void	WSU::accessLog( String info )
{
	std::cout << BLUE << WSU::logDate() << RESET << " " << info << "\n";
}

std::string WSU::buildIMFDate() {
	char		buffer[30];
	std::time_t	t = std::time(nullptr); // Get the current time
	std::tm		*tm = std::gmtime(&t);	// Convert time to GMT (UTC)
	// Format the date in IMF format: Day, DD Mon YYYY HH:MM:SS GMT
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	std::strftime(buffer, sizeof(buffer), "[%d/%b/%Y:%H:%M:%S]", tm);
	return std::string(buffer); // Return the date as a string
}

void	WSU::tout( String __log_message )
{
	std::cout << __log_message << GREEN << "\t\t\t" << "OK" << RESET << std::endl;
}

void	WSU::terr( char *__error_message )
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}

void	WSU::terr( String __error_message )
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}

void	WSU::file_error( String __error_message, String __file, String __line )
{
	std::cerr << RED << __file << ":" << __line << ": error: " << RESET << __error_message << std::endl;
}

void		WSU::l1()
{
	std::cout << BLUE << WSU::logDate() << RESET << " ";
}
void		WSU::l1( String s )
{
	std::cout << " " << s;
}
void		WSU::l1( int i )
{
	std::cout << " " << i;
}

void WSU::trimSpaces(String &str)
{
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
