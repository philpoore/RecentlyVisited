#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(std::string msg);

class ServerSocket {
    typedef struct sockaddr_in SocketAddr;

    int sock_fd = -1;

  public:
    ServerSocket();
    ~ServerSocket();
    void bind(int port);

    template <class F> void listen(F f) {
        // Accept loop
        int client_fd;
        struct sockaddr client_addr;
        socklen_t client_addr_size = sizeof(client_addr);

        std::cout << "Waiting for connections...\n";
        while ((client_fd = accept(sock_fd, &client_addr, &client_addr_size))) {
            if (client_fd == -1) {
                perror("accept error");
                continue;
            }
            f(client_fd, client_addr);
        }
    }
};