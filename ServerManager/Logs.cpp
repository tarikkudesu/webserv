#include "Logs.hpp"

Logs::Logs() {}

Logs::Logs( const Logs &copy ) { (void) copy; }

Logs::~Logs() {}

Logs	&Logs::operator=( const Logs &assign )
{
	(void) assign;
	return *this;
}

std::string Logs::buildIMFDate() {
	char		buffer[30];
	std::time_t	t = std::time(nullptr); // Get the current time
	std::tm		*tm = std::gmtime(&t);	// Convert time to GMT (UTC)
	// Format the date in IMF format: Day, DD Mon YYYY HH:MM:SS GMT
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buffer); // Return the date as a string
}

void	Logs::tout( String __log_message )
{
	std::cout << __log_message << GREEN << "\t\t\t" << "OK" << RESET << std::endl;
}

void	Logs::terr( char *__error_message )
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}

void	Logs::terr( String __error_message )
{
	std::cerr << RED << "error: " << RESET << __error_message << std::endl;
}

void	Logs::file_error( String __error_message, String __file, String __line )
{
	std::cerr << RED << __file << ":" << __line << ": error: " << RESET << __error_message << std::endl;
}
