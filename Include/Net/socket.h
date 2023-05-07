#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include "Include/Net/socketpack.h"
#include "Include/Net/inetaddress.h"
#include <string>
#include <netinet/tcp.h>

namespace wasi {
namespace net {

class Socket {
private:
    bool connected_;
    const int sockfd_;

public:
    Socket(int sockfd);
    void Bind(const InetAddress& localaddr);
    void Listen();
    int Accept(InetAddress* peeraddr);
    int GetFd() const;
    bool GetTcpInfo(std::string* str) const;
    bool GetTcpInfo(tcp_info* tcpi) const;
    void CloseWrite();
    bool SetTcpDelay(bool flag);
    int SetReuseAddr(bool flag);
    int SetReusePort(bool flag);
    bool SetKeepAlive(bool flag);
    ~Socket();
};

}} // namespace wasi::net

#endif
//