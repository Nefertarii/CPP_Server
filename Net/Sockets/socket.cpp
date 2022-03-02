#include "Head/socket.h"
#include "Head/inetaddress.h"
#include <netinet/in.h>
#include <netinet/tcp.h>

using namespace Wasi::Sockets;

Socket::Socket(int sockfd_) :sockfd(sockfd_) {}

int Fd() const { return sockfd; }

void Bind_address(const InetAddress& localaddr) {
    
}

void Listen() {}
int Accept(InetAddress* peeraddr) {}
void Shutdown_write() {}
void Set_tcp_delay(bool status) {}
void Set_reuse_addr(bool status) {}
void Set_reuse_port(bool status) {}
void Set_keep_alive(bool status) {}

~Socket() {
    if()
}