#include "socketapi.h"
#include "../Log/logging.h"
#include <cassert>
#include <cstring>
#include <endian.h>
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace Wasi {
namespace Sockets {

int Create_socket(sa_family_t family) {
    int sockfd = socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0) {
        LOG_FATAL("Fatal");
    }
    return sockfd;
}

int Accept(int sockfd, sockaddr_in6* addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr));
    int connfd        = accept4(sockfd, Sockaddr_cast(addr),
                                &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        int tmp_errno       = errno;
        std::string str_err = "Error, ";
        switch (tmp_errno) {
        case EINTR: str_err += "EINTR";
        case EPERM: str_err += "EPERM";
        case EBADF: str_err += "EBADF";
        case EAGAIN: str_err += "EAGAIN";
        case EPROTO: str_err += "EPROTO";
        case EMFILE: str_err += "EMFILE";
        case EFAULT: str_err += "EFAULT";
        case EINVAL: str_err += "EINVAL";
        case ENFILE: str_err += "ENFILE";
        case ENOMEM: str_err += "ENOMEM";
        case ENOBUFS: str_err += "ENOBUFS";
        case ENOTSOCK: str_err += "ENOTSOCK";
        case EOPNOTSUPP: str_err += "EOPNOTSUPP";
        case ECONNABORTED: str_err += "ECONNABORTED";
        default:
            str_err += "Unexpected error\n";
        }
        LOG_ERROR(str_err);
    }
    return connfd;
}

int Connect(int sockfd, const sockaddr* addr) {
    return connect(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6)));
}

void Bind(int sockfd, const sockaddr* addr) {
    int ret = bind(sockfd, addr, static_cast<socklen_t>(sizeof(sockaddr_in6)));
    if (ret < 0) {
    }
}

void Listen(int sockfd) {
    int ret = listen(sockfd, SOMAXCONN);
    if (ret < 0) {
        LOG_ERROR("Error");
    }
}

ssize_t Read(int sockfd, void* buf, size_t count) {
    return read(sockfd, buf, count);
}

/*ssize_t Read_val(int sockfd, const iovec* iov, int iovcnt) {
    return readv()
}*/

ssize_t Write(int sockfd, const void* buf, size_t count) {
    return write(sockfd, buf, count);
}

void Shutdown_write(int sockfd) {
    if (shutdown(sockfd, SHUT_WR) < 0) {
        LOG_ERROR("Error");
    }
}

void Shutdown_read(int sockfd) {
    if (shutdown(sockfd, SHUT_RD) < 0) {
        LOG_ERROR("Error");
    }
}

void Close(int sockfd) {
    if (close(sockfd) < 0) {
        LOG_ERROR("Error");
    }
}

int Get_socket_error(int sockfd) {
    int optval       = 0;
    socklen_t optlen = static_cast<socklen_t>(sizeof(optval));
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen)) {
        return errno;
    } else {
        return optval;
    }
}

thread_local char err_buf[512];
thread_local char time[64];
thread_local time_t last_sec;
const char* String_error(int err) {
    return strerror_r(err, err_buf, sizeof(err_buf));
}

sockaddr_in6 Get_local_addr(int sockfd) {
    sockaddr_in6 localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sockfd, Sockaddr_cast(&localaddr), &addrlen) < 0) {
        LOG_ERROR("Error");
    }
    return localaddr;
}

sockaddr_in6 Get_peer_addr(int sockfd) {
    sockaddr_in6 peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof(peeraddr));
    if (getpeername(sockfd, Sockaddr_cast(&peeraddr), &addrlen) < 0) {
        LOG_ERROR("Error");
    }
    return peeraddr;
}

void From_ip_port(const char* ip, uint16_t port, sockaddr_in* addr) {
    addr->sin_family = AF_INET;
    addr->sin_port   = Host_to_network_16(port);
    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        LOG_ERROR("Error");
    }
}

void From_ip_port(const char* ip, uint16_t port, sockaddr_in6* addr) {
    addr->sin6_family = AF_INET6;
    addr->sin6_port   = Host_to_network_16(port);
    if (inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0) {
        LOG_ERROR("Error");
    }
}

void To_ip_port(char* buf, size_t size, const sockaddr* addr_) {
    if (addr_->sa_family == AF_INET6) {
        buf[0] = '[';
        To_ip(buf + 1, size - 1, addr_);
        size_t end                = strlen(buf);
        const sockaddr_in6* addr6 = Sockaddr_in6_cast(addr_);
        uint port                 = Network_to_host_16(addr6->sin6_port);
        assert(size > end);
        snprintf(buf + end, size - end, "]:%u", port);
    } else {
        To_ip(buf, size, addr_);
        size_t end               = strlen(buf);
        const sockaddr_in* addr4 = Sockaddr_in_cast(addr_);
        uint16_t port            = Network_to_host_16(addr4->sin_port);
        assert(size > end);
        snprintf(buf + end, size - end, ":%u", port);
    }
}

void To_ip(char* buf, size_t size, const sockaddr* addr_) {
    if (addr_->sa_family == AF_INET) {
        assert(size >= INET_ADDRSTRLEN);
        const sockaddr_in* addr4 = Sockaddr_in_cast(addr_);
        inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    } else if (addr_->sa_family == AF_INET6) {
        assert(size >= INET6_ADDRSTRLEN);
        const sockaddr_in6* addr6 = Sockaddr_in6_cast(addr_);
        inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}

const sockaddr_in* Sockaddr_in_cast(const sockaddr* addr) {
    return static_cast<const sockaddr_in*>(static_cast<const void*>(addr));
}

const sockaddr_in6* Sockaddr_in6_cast(const sockaddr* addr) {
    return static_cast<const sockaddr_in6*>(static_cast<const void*>(addr));
}

const sockaddr* Sockaddr_cast(const sockaddr_in* addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}

const sockaddr* Sockaddr_cast(const sockaddr_in6* addr) {
    return static_cast<const sockaddr*>(static_cast<const void*>(addr));
}

sockaddr* Sockaddr_cast(sockaddr_in6* addr) {
    return static_cast<sockaddr*>(static_cast<void*>(addr));
}
}
} // namespace Wasi::Sockets
