import socket
import sys

host = "localhost"
port = 9000



request = (
        "POST / HTTP/1.1\r\n"
        "Host: localhost:9000\r\n"
        "Connection: keep-alive\r\n"
        "Cookie: this is a delisious cookie\r\n"
        "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36\r\n"
        "\r\n"
)
print("\033[1;33m--------------------------------------------------")
print(request)
print("\033[1;32m--------------------------------------------------")
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())  # Sending the request to the server
    response = sock.recv(4096)  # Receiving the response
    print(response.decode())  # Decoding and printing the response
print("--------------------------------------------------\033[0m")
