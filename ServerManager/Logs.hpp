#ifndef LOGS_HPP
# define LOGS_HPP

# include "../srcs/webserv.hpp"

class Logs
{
	private :
		Logs();
		Logs( const Logs &copy );
		Logs	&operator=( const Logs &assign );
		~Logs();

	public:

		static void	tout( String __log_message );
		static void	terr( char *__error_message );
		static void	terr( String __error_message );
		static void	file_error( String __error_message, String __file, String __line );
};

#endif
