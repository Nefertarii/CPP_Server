#include "Include/Net/socketpack.h"
#include "Include/Logger/logger.h"
#include <cassert>
#include <endian.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstring>

using namespace wasi;
using namespace wasi::net;

int SocketPack::CreateSocket(sa_family_t family) {
    int sockfd = socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0) {
        LOG_CRIT("errno:" + std::to_string(errno));
    }
    return sockfd;
}

void SocketPack::Bind(int sockfd, const sockaddr* addr) {
    if (bind(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6))) < 0) {
        LOG_CRIT("errno:" + std::to_string(errno));
    }
}

void SocketPack::Listen(int sockfd) {
    if (listen(sockfd, SOMAXCONN) < 0) {
        LOG_CRIT("errno:" + std::to_string(errno));
    }
}

int SocketPack::Connect(int sockfd, const sockaddr* addr) {
    return connect(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6)));
}

int SocketPack::Accept(int sockfd, sockaddr_in6* addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr));
    int connfd        = accept4(sockfd, SockaddrCast(addr),
                                &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
    return connfd;
}

ssize_t SocketPack::Read(int sockfd, void* buf, size_t count) {
    ssize_t ret = read(sockfd, buf, count);
    if (ret < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
    return ret;
}

ssize_t SocketPack::Write(int sockfd, const void* buf, size_t count) {
    ssize_t ret = write(sockfd, buf, count);
    if (ret < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
    return ret;
}

void SocketPack::CloseWrite(int sockfd) {
    if (shutdown(sockfd, SHUT_WR) < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
}

void SocketPack::CloseRead(int sockfd) {
    if (shutdown(sockfd, SHUT_RD) < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
}

void SocketPack::Close(int sockfd) {
    if (close(sockfd) < 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
}

int SocketPack::GetSocketError(int sockfd) {
    int optval       = 0;
    socklen_t optlen = static_cast<socklen_t>(sizeof(optval));
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen)) {
        return errno;
    } else {
        return optval;
    }
}

const char* SocketPack::StringError(int err) {
    char err_buf[512];
    return strerror_r(err, err_buf, sizeof(err_buf));
}

sockaddr_in6 SocketPack::GetLocalAddr(int sockfd) {
    sockaddr_in6 localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sockfd, SockaddrCast(&localaddr), &addrlen) != 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
    return localaddr;
}

sockaddr_in6 SocketPack::GetPeerAddr(int sockfd) {
    sockaddr_in6 peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof(peeraddr));
    if (getpeername(sockfd, SockaddrCast(&peeraddr), &addrlen) != 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
    return peeraddr;
}

void SocketPack::FromIpPort(const char* ip, uint16_t port, sockaddr_in* addr) {
    addr->sin_family = AF_INET;
    addr->sin_port   = HostToNetwork16(port);
    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
}

void SocketPack::FromIpPort(const char* ip, uint16_t port, sockaddr_in6* addr) {
    addr->sin6_family = AF_INET6;
    addr->sin6_port   = HostToNetwork16(port);
    if (inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0) {
        LOG_ERROR("errno:" + std::to_string(errno));
    }
}

void SocketPack::ToIpPort(char* buf, size_t size, const sockaddr* addr) {
    if (addr->sa_family == AF_INET6) {
        buf[0] = '[';
        ToIp(buf + 1, size - 1, addr);
        size_t end                = strlen(buf);
        const sockaddr_in6* addr6 = SockaddrIn6Cast(addr);
        uint port                 = HostToNetwork16(addr6->sin6_port);
        assert(size > end);
        snprintf(buf + end, size - end, "]:%u", port);
    } else {
        ToIp(buf, size, addr);
        size_t end               = strlen(buf);
        const sockaddr_in* addr4 = SockaddrInCast(addr);
        uint16_t port            = HostToNetwork16(addr4->sin_port);
        assert(size > end);
        snprintf(buf + end, size - end, ":%u", port);
    }
}

void SocketPack::ToIp(char* buf, size_t size, const sockaddr* addr) {
    if (addr->sa_family == AF_INET) {
        assert(size >= INET_ADDRSTRLEN);
        const sockaddr_in* addr4 = SockaddrInCast(addr);
        inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    } else if (addr->sa_family == AF_INET6) {
        assert(size >= INET6_ADDRSTRLEN);
        const sockaddr_in6* addr6 = SockaddrIn6Cast(addr);
        inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}

const sockaddr_in* SocketPack::SockaddrInCast(const sockaddr* addr) {
    return static_cast<const sockaddr_in*>(static_cast<const void*>(addr));
}

const sockaddr_in6* SocketPack::SockaddrIn6Cast(const sockaddr* addr) {
    return static_cast<const sockaddr_in6*>(static_cast<const void*>(addr));
}

const sockaddr* SocketPack::SockaddrCast(const sockaddr_in* addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}

const sockaddr* SocketPack::SockaddrCast(const sockaddr_in6* addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}

sockaddr* SocketPack::SockaddrCast(sockaddr_in6* addr) {
    return static_cast<sockaddr*>(static_cast<void*>(addr));
}