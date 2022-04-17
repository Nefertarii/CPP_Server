#ifndef SOCKTES_CONNECTOR_H_
#define SOCKTES_CONNECTOR_H_

#include "../../../Class/noncopyable.h"
#include "inetaddress.h"
#include <functional>
#include <memory>

namespace Wasi {
namespace Poll {
class Channel;
class EventLoop;
} // namespace Poll
namespace Sockets {
using NewConnectionCallback = std::function<void(int)>;

class Connector : Noncopyable,
                  public std::enable_shared_from_this<Connector> {
private:
    enum States {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };
    static const int max_retry_delay_ms;
    static const int init_retry_delay_ms;
    void Set_state(States state_);
    void Start_in_loop();
    void Stop_in_loop();
    void Connect();
    void Connecting(int sockfd);
    void Handle_write();
    void Handle_error();
    void Retry(int sockfd);
    void Reset_channel();
    int Remove_and_Reset();
    std::string Str_state();
    Poll::EventLoop* loop;
    InetAddress servaddr;
    int retry_delay_ms;
    bool connect;
    States state;
    std::unique_ptr<Poll::Channel> channel;
    NewConnectionCallback new_connection_callback;

public:
    Connector(Poll::EventLoop* loop_,
              const InetAddress& servaddr_);
    void Set_new_connection_callback(const NewConnectionCallback& callback);
    void Start();
    void Restart();
    void Stop();
    const InetAddress& Get_servaddr() const;
    ~Connector();
};
} // namespace Sockets
} // namespace Wasi

#endif