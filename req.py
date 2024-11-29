import socket

host = "localhost"
port = 443

# Construct the HTTP request
request = (
    # "POST /api/login HTTP/1.1\r\n"
    # "Host: example.com\r\n"
    # "Content-Type: application/json\r\n"
    # "Content-Length: 53\r\n"
    # "\r\n"
    # "{\"username\": \"exampleUser\", \"password\": \"examplePass\"}"
    "PUT /upload HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: text/plain\r\n"
    "Transfer-Encoding: chunked\r\n"
    "\r\n"
    "7\r\n"
    "Mozilla\r\n"
    "9\r\n"
    "Developer\r\n"
    "7\r\n"
    "Network\r\n"
    "0\r\n"
    "\r\n"
)

# Send the request
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())

