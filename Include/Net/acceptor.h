#ifndef NET_ACCEPTOR_H_
#define NET_ACCEPTOR_H_

#include "Include/Net/inetaddress.h"
#include "Include/Net/socket.h"
#include "Include/Eventloop/channel.h"
#include <functional>

namespace wasi {
namespace loop {
class EventLoop;
} // namespace loop

namespace net {

using NewConnectCallback = std::function<void(int, const InetAddress&)>;

class Acceptor {
private:
    Socket socket_;
    loop::EventLoop* eventloop_;
    loop::Channel channel_;
    NewConnectCallback connected_func;
    bool listening_;
    int idle_fd_;
    void HandleRead();

public:
    Acceptor(loop::EventLoop* eventloop, const InetAddress& listen_addr, bool reuse_port = true);
    void SetNewConnectionCallback(const NewConnectCallback& func);
    void Listen();
    bool Listening() const;
    ~Acceptor();
};

} // namespace net
} // namespace wasi

#endif