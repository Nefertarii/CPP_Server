#include "socket.h"
#include <Log/logging.h>
#include "inetaddress.h"
#include "socketapi.h"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
using namespace Wasi::Sockets;

Socket::Socket(int sockfd_) :
    sockfd(sockfd_) {}

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
        LOG_ERROR("Reuse port fail.");
    }
}

void Socket::Set_keep_alive(bool status) {
    int opt = status ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE,
               &opt, static_cast<socklen_t>(sizeof(opt)));
}

bool Socket::Get_tcp_info(std::string* buf) const {
    tcp_info tcpi;
    std::string tmp_str;
    if (Get_tcp_info(&tcpi)) {
        tmp_str = "unrecovered = ";
        tmp_str += std::to_string(tcpi.tcpi_retransmits);
        tmp_str += "rto = " + std::to_string(tcpi.tcpi_rto);
        tmp_str += "ato = " + std::to_string(tcpi.tcpi_ato);
        tmp_str += "snd_mss = " + std::to_string(tcpi.tcpi_snd_mss);
        tmp_str += "rcv_mss = " + std::to_string(tcpi.tcpi_rcv_mss);
        tmp_str += "lost = " + std::to_string(tcpi.tcpi_lost);
        tmp_str += "retrans = " + std::to_string(tcpi.tcpi_retrans);
        tmp_str += "rtt = " + std::to_string(tcpi.tcpi_rtt);
        tmp_str += "rttvar = " + std::to_string(tcpi.tcpi_rttvar);
        tmp_str += "sshthresh = " + std::to_string(tcpi.tcpi_snd_ssthresh);
        tmp_str += "cwnd = " + std::to_string(tcpi.tcpi_snd_cwnd);
        tmp_str += +" total_retrans = " + std::to_string(tcpi.tcpi_total_retrans);
        *buf = tmp_str;
        return true;
    }
    return false;
}

bool Socket::Get_tcp_info(tcp_info* tcpi) const {
    socklen_t len = sizeof(*tcpi);
    memset(tcpi, 0, sizeof(*tcpi));
    return getsockopt(sockfd, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

Socket::~Socket() {
    Close(sockfd);
}