#ifndef NET_CONNECTOR_H_
#define NET_CONNECTOR_H_

#include "Include/Net/inetaddress.h"
#include <functional>
#include <memory>

namespace wasi {
namespace loop {
class EventLoop;
class Channel;

} // namespace loop
namespace net {

enum ConnectorState {
    kConnectordisconnected,
    kConnectorconnecting,
    kConnectorconnected
};

using NewConnectionCallback = std::function<void(int)>;

class Connector {
private:
    static const int max_retry_delay_;  // ms
    static const int init_retry_delay_; // ms
    void SetState(ConnectorState state);
    std::string StrState(ConnectorState state);
    void StartInLoop();
    void StopInLoop();
    void Connect();
    void Connecting(int sockfd);
    void HandleWrite();
    void HandleError();
    void Retry(int sockfd);
    void ResetChannel();
    int RemoveAndReset();
    loop::EventLoop* loop_;
    InetAddress peer_addr_;
    int retry_delay_; // ms
    bool connect_;
    ConnectorState state_;
    std::unique_ptr<loop::Channel> channel_;
    NewConnectionCallback new_connection_callback_;

public:
    Connector(loop::EventLoop* loop, const InetAddress& peer_addr);
    void SetNewConnectionCallback(const NewConnectionCallback& func);
    void Start();
    void Restart();
    void Stop();
    const InetAddress& GetPeerAddr() const;
    ~Connector();

};

} // namespace net
} // namespace wasi

#endif