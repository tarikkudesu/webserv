import socket
import sys

host = "localhost"
port = 9000



request = (
        "POST / HTTP/1.1\r\n"
        "Host: localhost:9000\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 1555\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36\r\n"
        "\r\n"
        "#ifndef __LOCATION_HPP__\n"
        "# define __LOCATION_HPP__\n"
        "\n"
        "class Location\n"
        "{\n"
        "	private :\n"
        "		bool								b__r; // temporary usage\n"
        "\n"
        "		void								proccessClientBodyBufferSizeToken( t_svec &tokens );\n"
        "		void								proccessAllowMethodsDirective( t_svec &tokens );\n"
        "		void								proccessErrorPageDirective( t_svec &tokens );\n"
        "		void								proccessAutoindexDirective( t_svec &tokens );\n"
        "		void								proccessCgiPassDirective( t_svec &tokens );\n"
        "		void								proccessReturnDirective( t_svec &tokens );\n"
        "		void								proccessIndexDirective( t_svec &tokens );\n"
        "		void								proccessRootDirective( t_svec &tokens );\n"
        "		void								proccessToken(t_svec &tokens );\n"
        "		void								LocationBlock( size_t pos );\n"
        "		void								addDirective( size_t end );\n"
        "		void								proccessDirectives();\n"
        "		void								parseDirectives();\n"
        "		void								parse();\n"
        "		Location();\n"
        "\n"
        "	public:\n"
        "		const String						__path;\n"
        "		String								__line; // temporary usage\n"
        "		String								__root;\n"
        "		t_svec								__index;\n"
        "		String								__return;\n"
        "		String								__cgiPass;\n"
        "		bool								__autoindex;\n"
        "		std::deque< String >				__directives;\n"
        "		std::map< int16_t, String >			__errorPages;\n"
        "		std::vector< t_method >				__allowMethods;\n"
        "		long								__clientBodyBufferSize;\n"
        "\n"
        "\n"
        "		Location( const Location &copy );\n"
        "		Location( const String &conf, const String &root );\n"
        "		Location( const String &dir, const String &conf, const String &root );\n"
        "		Location	&operator=( const Location &assign );\n"
        "		~Location();\n"
        "};\n"
        "\n"
        "String methodToString(t_method t);\n"
        "std::ostream &operator<<( std::ostream &o, const Location &loc );\n"
        "\n"
        "#endif\n"
        "\n"
)
print("\033[1;33m--------------------------------------------------")
# print(request)
print("\033[1;32m--------------------------------------------------")
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())  # Sending the request to the server
    response = sock.recv(4096)  # Receiving the response
    print(response.decode())  # Decoding and printing the response
print("--------------------------------------------------\033[0m")
