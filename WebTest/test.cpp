#include "../Debug/Important/Gsocketfunc.h"
#include <iostream>
#include <string>

int main() {

    Log* log = new Log("test.txt", 200);
    int socketfd = 0;
    struct sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    socketfd = Gsocket::Socket(AF_INET, SOCK_STREAM, 0, log);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8100);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    connect(socketfd, (sockaddr *)&servaddr, sizeof(servaddr));
    signal(SIGPIPE, SIG_IGN);
    
    int t;
    std::string GET1 = "GET/HTTP/1.1\n";
    std::string GET2 = "GET/Login/login.html HTTP/1.1\n";
    std::string GET3 = "GET/Image/ico/favicon.ico HTTP/1.1\n";
    while(1) {
        std::cout << "input:";
        std::cin >> t;
        switch (t) {
        case 1:
            std::cout << "send:" << GET1;
            Gsocket::Write(socketfd, &GET1, log);
            break;
        case 2:
            std::cout << "send:" << GET2;
            Gsocket::Write(socketfd, &GET2, log);
            break;
        case 3:
            std::cout << "send:" << GET3;
            Gsocket::Write(socketfd, &GET3, log);
            break;
        default:
            std::cout << "bad input\n";
            break;
        }
        Gsocket::Read(socketfd, &recv, 4000, log);
        std::cout << recv;
    }
    delete log;
}