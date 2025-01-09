#ifndef __HEADERS_HPP__
# define __HEADERS_HPP__

# include "PackageResponse/ErrorResponse.hpp"

class Headers
{
	private:
		void							clear();
        String                          getHeaderFeildValue(const String &key, std::map< String, String > &headers);

	public:
		t_transferEncoding				__transferEncoding;
		t_connectionType				__connectionType;
		size_t							__contentLength;
		String							__host;
		int								__port;

		void							parseHeaders(std::map< String, String > &headers);

		Headers();
		Headers( const Headers &copy );
		Headers& operator=( const Headers &assign );
		~Headers();
};

#endif
