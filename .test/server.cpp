
# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <string.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <std::vector>
# include <map>
# include <iomanip>
# include <algorithm>
# include <sys/poll.h>

# define SERVER_PORT    1800
# define MAX_CLIENTS    200
# define MAXLINE		4096
# define SA				struct sockaddr

void    terror( std::string __error_message ) {
    std::cerr << __error_message << "\n";
    perror("err ");
    exit(EXIT_FAILURE);
}

// a NON blocking webserver using poll
int main() {
    std::cout << std::unitbuf; //enable instant buffer fulshing
	std::cerr << std::unitbuf; //enable instant buffer fulshing

    int                 rv; // return value
    int                 listenSd = -1;
    int                 newSd = -1;
    struct sockaddr_in  addr;
    struct pollfd       sds[MAX_CLIENTS];

    // create socket
    listenSd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSd < 0)
        terror("socket syscall");

    // make the socket reusable
    int reuse = 1;
    rv = setsockopt(listenSd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse));
    if (rv < 0)
        terror("setsockopt syscall");
    
    // set the socket to non blocking mode, newly created sockets will inherit this property
    int flags = fcntl(listenSd, F_GETFL, 0);
    if (flags < 0 || fcntl(listenSd, F_SETFL, flags | O_NONBLOCK) < 0)
        terror("fcntl syscall");


    // set address structure and bind the socket to a port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);
    rv = bind(listenSd, (SA *)&addr, sizeof(addr));
    if (rv < 0)
        terror("bind syscall");

    // start listening for incoming connections
    rv = listen(listenSd, 10);
    if (rv < 0)
        terror("listen syscall");

    memset(sds, 0, sizeof(sds));
    // setup the initial listening socket
    sds[0].fd = listenSd;
    sds[0].events = POLLIN;
    for (int i = 1; i < MAX_CLIENTS; i++)
        sds[i].fd = -1;

    char    buff[LINE_MAX];

    int           nfds = 1, current_size = 0;
    for ( ; ; ) {
        usleep(1000);
        std::cout << "Waiting for connection ...\n";
        rv = poll(sds, nfds, 0);
        if (rv < 0) {
            perror("poll syscall err");
            continue;
        }
        current_size = nfds;
        for (int fd = 0; fd < current_size; fd++) {
            bool    close_conn = false;
            if (sds[fd].revents == 0)
                continue ;
            if (sds[fd].fd == listenSd) {
                do {
                    std::cout << "listening socket available and ready to connect\n";
                    newSd = accept(listenSd, NULL, NULL);
                    if (newSd < 0) {
                        if (errno != EWOULDBLOCK)
                            terror("accept syscall");
                        else
                            break ;
                    }
                    sds[nfds].fd = newSd;
                    sds[nfds].events = POLL_IN;
                    nfds++;
                } while (newSd != 1);
            } else {
                std::cout << "readable socket\n";
                rv = recv(sds[fd].fd, buff, sizeof(buff), 0);
                if (rv == 0) {
                    close_conn = true;
                    std::cout << "conection closed\n";
                } else if (rv > 0) {

                }
                
            }
        }
    }
    close(listenSd);
    return 0;
}

/*
int main() {
    std::cout << std::unitbuf; //enable instant buffer fulshing
	std::cerr << std::unitbuf; //enable instant buffer fulshing

    int                 listenfd, connfd;
    struct sockaddr_in  servaddr;
    uint8_t             buff[MAXLINE + 1];
    uint8_t             recvline[MAXLINE + 1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket err: ");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // INADDR_ANY == respond to anything
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind err: ");
        return 1;
    }
    if (listen(listenfd, 10) < 0) {
        perror("listen err: ");
        return 1;
    }

    for ( ; ; ) {
        // struct sockaddr_in  addr;
        // socklen_t           addr_len;

        std::cout << "waiting for connections on " << SERVER_PORT << "\n";
        connfd = accept(listenfd, (SA *)NULL, NULL);

        memset(recvline, 0, MAXLINE);

        // while ( (n = read(connfd, recvline, MAXLINE - 1)) > 0) {
        //     std::cout << "reading " << recvline << "\n";
        //     if (std::string((char *)recvline) == "\n")
        //         break ;
        //     memset(recvline, 0, MAXLINE);
        // }
        // if (n < 0) {
        //     perror("read err: ");
        //     return 1;
        // }

        snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHELLO");

        write (connfd, (char *)buff, strlen((char *)buff));
        write (1, (char *)buff, strlen((char *)buff));
        close(connfd);
    }
}
*/