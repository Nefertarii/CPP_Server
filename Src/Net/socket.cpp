#include "Include/Net/socket.h"
#include <cstring>

using namespace wasi;
using namespace wasi::net;

Socket::Socket(int sockfd) :
    sockfd_(sockfd) {}

void Socket::Bind(const InetAddress& localaddr) {
    SocketPack::Bind(sockfd_, localaddr.GetSockaddr());
}

void Socket::Listen() {
    SocketPack::Listen(sockfd_);
}

int Socket::Accept(InetAddress* peeraddr) {
    sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = SocketPack::Accept(sockfd_, &addr);
    if (connfd >= 0) {
        peeraddr->SetSockaddrInet6(addr);
    }
    return connfd;
}

int Socket::GetFd() const { return sockfd_; }

bool Socket::GetTcpInfo(std::string* str) const {
    tcp_info tcpi;
    if (GetTcpInfo(&tcpi)) {
        str->clear();
        str->append("unrecovered = " + std::to_string(tcpi.tcpi_retransmits) + "\n");
        str->append("rto = " + std::to_string(tcpi.tcpi_rto) + "\n");
        str->append("ato = " + std::to_string(tcpi.tcpi_ato) + "\n");
        str->append("snd_mss = " + std::to_string(tcpi.tcpi_snd_mss) + "\n");
        str->append("rcv_mss = " + std::to_string(tcpi.tcpi_rcv_mss) + "\n");
        str->append("lost = " + std::to_string(tcpi.tcpi_lost) + "\n");
        str->append("retrans = " + std::to_string(tcpi.tcpi_retrans) + "\n");
        str->append("rtt = " + std::to_string(tcpi.tcpi_rtt) + "\n");
        str->append("rttvar = " + std::to_string(tcpi.tcpi_rttvar) + "\n");
        str->append("sshthresh = " + std::to_string(tcpi.tcpi_snd_ssthresh) + "\n");
        str->append("cwnd = " + std::to_string(tcpi.tcpi_snd_cwnd) + "\n");
        str->append("total_retrans = " + std::to_string(tcpi.tcpi_total_retrans) + "\n");
        return true;
    }
    return false;
}

bool Socket::GetTcpInfo(tcp_info* tcpi) const {
    socklen_t len = sizeof(*tcpi);
    memset(tcpi, 0, sizeof(*tcpi));
    return getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

void Socket::CloseWrite() {
    SocketPack::CloseWrite(sockfd_);
}

bool Socket::SetTcpDelay(bool flag) {
    int opt = flag ? 1 : 0;
    return setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
                      &opt, static_cast<socklen_t>(sizeof(opt)));
}

int Socket::SetReuseAddr(bool flag) {
    int opt = flag ? 1 : 0;
    return setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                      &opt, static_cast<socklen_t>(sizeof(opt)));
}

int Socket::SetReusePort(bool flag) {
    int opt = flag ? 1 : 0;
    return setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                      &opt, static_cast<socklen_t>(sizeof(opt)));
}

bool Socket::SetKeepAlive(bool flag) {
    int opt = flag ? 1 : 0;
    return setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                      &opt, static_cast<socklen_t>(sizeof(opt)));
}

Socket::~Socket() {
    SocketPack::Close(sockfd_);
}