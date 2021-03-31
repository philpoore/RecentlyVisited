#include "socket.h"

void error(std::string msg) {
    std::cout << "Error: " << msg << "\n";
    perror(nullptr);
    exit(1);
}

ServerSocket::ServerSocket() {
    // Create TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        error("Creating socket");
    }
}

ServerSocket::~ServerSocket() {
    if (sock_fd != -1)
        close(sock_fd);
}

void ServerSocket::bind(int port) {
    int enable = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    // Bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr))) {
        error("Binding socket");
    }

    // Listen
    int accept_backlog_size = 1;
    if (::listen(sock_fd, accept_backlog_size)) {
        error("Listening");
    }
}