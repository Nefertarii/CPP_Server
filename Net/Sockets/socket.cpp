#include "Head/socket.h"
#include "Head/socketapi.h"
#include "Head/inetaddress.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cstring>
#include <iostream>

using namespace Wasi::Sockets;

Socket::Socket(int sockfd_) :sockfd(sockfd_) {}

int Socket::Fd() const { return sockfd; }

void Socket::Bind_address(const InetAddress& localaddr) {
    Bind(sockfd, localaddr.Get_sockaddr());
}

void Socket::Listen() {
    Wasi::Sockets::Listen(sockfd);
}

int Socket::Accept(InetAddress* peeraddr) {
    sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = Wasi::Sockets::Accept(sockfd, &addr);
    if (connfd >= 0) {
        peeraddr->Set_sockaddr_inet6(addr);
    }
    return connfd;
}

void Socket::Shutdown_write() {
    Wasi::Sockets::Shutdown_write(sockfd);
}

void Socket::Set_tcp_delay(bool status) {
    int opt = status ? 1 : 0;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,
               &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Socket::Set_reuse_addr(bool status) {
    int opt = status ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
               &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Socket::Set_reuse_port(bool status) {
    int opt = status ? 1 : 0;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT,
                         &opt, static_cast<socklen_t>(sizeof(opt)));
    if (ret < 0 && status == true) {
        std::cout<<"Sockets::reuse port fail.\n";
    }
}

void Socket::Set_keep_alive(bool status) {
    int opt = status ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE,
               &opt, static_cast<socklen_t>(sizeof(opt)));
}

Socket::~Socket() {
    Close(sockfd);
}