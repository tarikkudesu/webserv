import socket

# Test with different boundary types
tests = [
    # Basic multipart form data
    (
        "POST /a.php  HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: multipart/form-data; boundary=---------------------------12345\r\n"
        "Content-Length: 374\r\n"
        "\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello, this is a test file!\r\n"
        "-----------------------------12345\r\n"
        "Content-Disposition: form-data; name=\"file2\"; filename=\"image.jpg\"\r\n"
        "Content-Type: image/jpeg\r\n"
        "\r\n"
        "Fake image data here\r\n"
        "-----------------------------12345--\r\n"
    ),
    # Empty form data
    (
        "POST /upload HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: multipart/form-data; boundary=---------------------------12345\r\n"
        "Content-Length: 49\r\n"
        "\r\n"
        "-----------------------------12345--\r\n"
    ),
    # Invalid boundary
    (
        "POST /upload HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: multipart/form-data; boundary=abc\r\n"
        "Content-Length: 100\r\n"
        "\r\n"
        "--abc\r\n"
        "Invalid format - no proper ending\r\n"
    )
]

def run_test(host, port, test_data):
    with socket.create_connection((host, port)) as sock:
        sock.sendall(test_data.encode())
        response = sock.recv(4096)
        return response.decode()

if __name__ == "__main__":
    HOST = "localhost"
    PORT = 8080
    
    for i, test in enumerate(tests, 1):
        print(f"\nTest {i}:")
        try:
            response = run_test(HOST, PORT, test)
            print(f"Response:\n{response}")
        except Exception as e:
            print(f"Error: {e}")
