#include <cassert>
#include <endian.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <signal.h>
#include <arpa/inet.h>
#include "../Net/Base/Head/buffer.h"

using namespace Wasi::Base;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9981);
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen)) < 0) {
        perror("accept");
    }
    Buffer buffers;
    int err = 0;
    buffers.Read_fd(new_socket, &err);
    std::cout << "Read:" << buffers.Content() << "\n";
}