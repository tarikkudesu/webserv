import socket
import sys

host = "localhost"
port = 9000



request = (
        "POST /a.php HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: multipart/form-data; boundary=---------------------------12345\r\n"
        "Content-Length: 374\r\n"
        "\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "11111111111111111111111111111111111111111111111111111111111111111111111111\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "22222222222222222222222222222222222222222222222222222222222222222222222222\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "33333333333333333333333333333333333333333333333333333333333333333333333333\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "4444444444444444444444444444444444444444444444444444444444444444444444444\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "5555555555555555555555555555555555555555555555555555555555555555555555555555\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file2\"; filename=\"image.jpg\"\r\n"
        "Content-Type: image/jpeg\r\n"
        "\r\n"
        "6666666666666666666666666666666666666666666666666666666666666666666666666666\r\n"
        "-----------------------------12345--\r\n"
)
print("\033[1;33m--------------------------------------------------")
# print(request)
print("\033[1;32m--------------------------------------------------")
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())  # Sending the request to the server
    response = sock.recv(4096)  # Receiving the response
    print(response.decode())  # Decoding and printing the response
print("--------------------------------------------------\033[0m")

