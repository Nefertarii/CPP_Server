#ifndef SOCKTES_ACCEPTOR_H_
#define SOCKTES_ACCEPTOR_H_

#include "channel.h"
#include "inetaddress.h"
#include "socket.h"
#include <Base/noncopyable.h>

namespace Wasi {
namespace Poll {
class EventLoop;
}

namespace Sockets {
using NewConnectCallback = std::function<void(int, const InetAddress&)>;

class Acceptor : Noncopyable {
private:
    void Handle_read();
    Poll::EventLoop* loop;  
    Socket accept_socket;
    Channel accept_channel;
    NewConnectCallback callback;
    bool listening;
    int idle_fd;

public:
    Acceptor(Poll::EventLoop* loop_, const InetAddress& listen_addr, bool reuse_port);
    void Set_new_connection_callback(const NewConnectCallback& callback_);
    void Listen();
    bool Listening() const;
    ~Acceptor();
};
} // namespace Sockets
} // namespace Wasi

#endif