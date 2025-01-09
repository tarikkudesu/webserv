#ifndef __HEADERS_HPP__
# define __HEADERS_HPP__

# include "PackageResponse/ErrorResponse.hpp"

class Headers
{
	private:
		void						clear();
		void						contentType( std::map< String, String > &headers );
		void						hostAndPort( std::map< String, String > &headers );
		void						contentLength( std::map< String, String > &headers );
		void						connectionType( std::map< String, String > &headers );
		void						transferEncoding( std::map< String, String > &headers );
		String						getHeaderFeildValue(const String &key, std::map< String, String > &headers);


	public:
		int							__port;
		String						__host;
		String						__boundry;
		t_transfer					__transfer;
		size_t						__contentLength;
		t_connectionType			__connectionType;

		void						parseHeaders(std::map< String, String > &headers);

		Headers();
		Headers( const Headers &copy );
		Headers& operator=( const Headers &assign );
		~Headers();
};

#endif
