import socket
import sys

host = "localhost"
port = 8080

request = ( # invalid method
    "GET / HTTP/1.1\r\n"
    "Host: domain0.com:443\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # invalid method
    "GET / HTTP/1.1\r\n"
    "Host: domain1.com:80\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())
exit()

request = ( # invalid Request Line
    "GET  HTTP/1.1\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: GENERAL\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # invalid URI invalid character
    "GET /<> HTTP/1.1\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # invalid URI extra space
    "GET / HTTP/1.1\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())
exit()

request = ( # unsupported protocole
    "GET /upload HTTP/1.2\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: GENERAL\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # oversized Header feild
    "GET /upload HTTP/1.1\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: GENERAL\r\n"
    "Key: value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_value_val\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # No Host Header feild
    "GET /upload HTTP/1.1\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: GENERAL\r\n"
    "\r\n"
)
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

request = ( # invalid chunk size
    "PUT /upload HTTP/1.1\r\n"
    "Host: localhost:444\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: chunked\r\n"
    "\r\n"
    "7\r\n"
    "Mozilla\r\n"
    "9\r\n"
    "Develo\r\n"
    "7\r\n"
    "Network\r\n"
    "0\r\n"
    "\r\n"
)

# Send the request

