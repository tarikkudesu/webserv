import socket

host = "localhost"
port = 443

# Construct the HTTP request
request = (
    "POST /path HTTP/1.1\r\n"
    "Host: yourserver.com\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "4\r\n"
    "Wiki\r\n"
    "5\r\n"
    "pedia\r\n"
    "E\r\n"
    " in\r\n"
    "chunks.\r\n"
    "0\r\n"
    "\r\n"
)

# Send the request
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

