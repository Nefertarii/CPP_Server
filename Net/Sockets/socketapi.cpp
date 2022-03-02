#include "Head/socketapi.h"
#include <cassert>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace Wasi::Sockets;

uint64_t Host_to_network_64(uint64_t host64) {
    return htobe64(host64);
}

uint32_t Host_to_network_32(uint32_t host32) {
    return htobe32(host32);
}

uint16_t Host_to_network_16(uint16_t host16) {
    return htobe16(host16);
}

uint64_t Network_to_host_64(uint64_t net64) {
    return be64toh(net64);
}

uint32_t Network_to_host_32(uint32_t net32) {
    return be32toh(net32);
}

uint16_t Network_to_host_16(uint16_t net16) {
    return be16toh(net16);
}

int Accept(int sockfd, sockaddr_in6 addr) {}

int Connect(int sockfd, const sockaddr* addr) {
    connect(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6));)
}

ssize_t Read(int sockfd, void* buf, size_t count) {
    return read(sockfd, buf, count);
}

/*ssize_t Read_val(int sockfd, const iovec* iov, int iovcnt) {
    return readv()
}*/

ssize_t Write(int sockfd, const void* buf, size_t count) {
    write(sockfd, buf, count);
}

void Close(int sockfd) {
    if (close(sockfd) < 0) {
        std::cout << "Sockets::Close error\n";
    }
}

void From_ip_port(const char* ip, uint16_t port, sockaddr_in* addr) {
    addr->sin_family = AF_INET;
    addr->sin_port = Host_to_network_16(port);
    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        std::cout << "From ip port err.\n";
    }
}

void From_ip_port(const char* ip, uint16_t port, sockaddr_in6* addr) {
    addr->sin6_family = AF_INET6;
    addr->sin6_port = Host_to_network_16(port);
    if (inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0) {
        std::cout << "From ip port err.\n";
    }
}

void To_ip_port(char* buf, size_t size, const sockaddr* addr_) {
    if (addr->sa_family == AF_INET6) {
        buf[0] = "[";
        To_ip(buf + 1, size - 1, addr_);
        size_t end = sizeof(buf) / sizeof(char);
        const sockaddr_in6* addr6 = sockaddr_in6_cast(addr_);
        uint port = Network_to_host_16(addr6->sin6_port);
        assert(size > end);
        snprintf(buf + end, size - end, "]:%u", port);
    }
    else {
        To_ip(buf, size, addr_);
        size_t end = sizeof(buf) / sizeof(char);
        const sockaddr_in* addr = sockaddr_in_cast(addr_);
        uint16_t port = Network_to_host_16(addr->sin_port);
        assert(size > end);
        snprintf(buf + end, size - end, ":%u", port);
    }
}

void To_ip(char* buf, size_t size, const sockaddr* addr_) {
    if (addr->sa_family == AF_INET) {
        assert(size >= INET_ADDRSTRLEN);
        const sockaddr_in* addr = sockaddr_in_cast(addr_);
        inet_ntop(AF_INET, &addr->sin_addr, buf, static_cast<socklen_t>(size));
    }
    else if(addr->sa_family == AF_INET6) {
        assert(size >= INET6_ADDRSTRLEN);
        const sockaddr_in6* addr6 = sockaddr_in6_cast(addr_);
        inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}

const sockaddr_in* sockaddr_in_cast(const sockaddr* addr) {
    static_cast<const sockaddr_in*>(static_cast<const void*>(addr));
}

const sockaddr_in6* sockaddr_in6_cast(const sockaddr* addr) {
    static_cast<const sockaddr_in6*>(static_cast<const void*>(addr));
}

const sockaddr* sockaddr_cast(const sockaddr_in addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}

const sockaddr* sockaddr_cast(const sockaddr_in6 addr) {
    return static_cast<struct sockaddr*>(static_cast<void*>(addr));
}

sockaddr* sockaddr_cast(const sockaddr_in6* addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}