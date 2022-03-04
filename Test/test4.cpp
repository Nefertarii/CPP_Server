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

int main() {
    int socketfd = 0;
    struct sockaddr_in servaddr;
    std::string recvstr, send;
    char recv[4096] = { 0 };
    memset(&servaddr, 0, sizeof(servaddr));
    socketfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9002);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    connect(socketfd, (sockaddr *)&servaddr, sizeof(servaddr));
    signal(SIGPIPE, SIG_IGN);
    
    int t;
    std::string GET1 = "hello1\n";
    std::string GET2 = "hello!!\n";
    std::string GET3 = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890~!@#$%^&*(){}:\">?\n";
    while (1) {
        memset(recv, 0, sizeof(recv));
        std::cout << "input:";
        std::cin >> t;
        switch (t) {
        case 1:
            std::cout << "send:" << GET1;
            write(socketfd, GET1.c_str(), GET1.size());
            break;
        case 2:
            std::cout << "send:" << GET2;
            write(socketfd, GET2.c_str(), GET2.size());
            break;
        case 3:
            std::cout << "send:" << GET3;
            write(socketfd, GET3.c_str(), GET3.size());
            break;
        default:
            std::cout << "bad input\n";
            break;
        }
        int readsize = read(socketfd, recv, 4096);
        recvstr = recv;
        std::cout << "Readsize:" << readsize << ", String:" << recvstr << "\n";
    }
}