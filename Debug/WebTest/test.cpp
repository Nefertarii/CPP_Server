#include "../Mainprocess/Httprocess/Servfunc.h"
#include <stdio.h>

int SERV::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        std::cout << "Socket error.\n";
        return -1;
    }
    std::cout << "Socket ok\n";
    return socketfd;
}


int SERV::Read(int socketfd, std::string *str) {
    char readbuf_tmp[4096] = {0};
    int readsize = read(socketfd, readbuf_tmp, 4096);
    if (readsize < 0) {
        if (errno == EINTR) {
            std::cout << "Read error, Single break\n";
        }
        else if(errno == EPIPE) {
            std::cout << "Read error, Client close\n";
        }
        else if(errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cout << "Read error\n";
        }
        return -1; // error close
    }
    else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close\n";
        std::cout << log;
        return 1; // normal close
    }
    else {
        std::string log = "Read from " + std::to_string(socketfd) + "\n";
        std::cout << log;
        *str = readbuf_tmp;
        return 0;
    }
}


int SERV::Write(int socketfd, std::string *str) {
    const char *tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    std::cout<<log<<"\n";
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            std::cout << "Write error\n"; //Signal interruption: slow system call
            return 1;
        }
        else if(errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cout << "write error\n"; //kernel cache full
            return 2;
        }
        else {
            std::cout << "write error\n";
            return -1;
        }
    }
    log += " success.\n";
    std::cout << log;
    return 0;
}





int main() {


    int socketfd = 0;
    struct sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    socketfd = SERV::Socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    inet_pton(AF_INET, "159.75.51.91", &servaddr.sin_addr);
    connect(socketfd, (sockaddr *)&servaddr, sizeof(servaddr));
    signal(SIGPIPE, SIG_IGN);

    int t;
    std::string GET1 = "GET / HTTP/1.1";
    std::string GET2 = "GET /Login/login.html HTTP/1.1";
    std::string GET3 = "GET /Image/ico/favicon.ico HTTP/1.1";
    while(1) {
        std::cout << "input:";
        std::cin >> t;
        switch (t) {
        case 1:
            std::cout << "send:" << GET1;
            SERV::Write(socketfd, &GET1);
            break;
        case 2:
            std::cout << "send:" << GET2;
            SERV::Write(socketfd, &GET2);
            break;
        case 3:
            std::cout << "send:" << GET3;
            SERV::Write(socketfd, &GET3);
            break;
        default:
            std::cout << "bad input\n";
            break;
        }
        SERV::Read(socketfd, &recv);
        std::cout << recv;
    }
}