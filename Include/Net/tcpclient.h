#ifndef NET_TCPCLIENT_H_
#define NET_TCPCLIENT_H_

#include "Include/Net/tcpconnection.h"
#include <mutex>

namespace wasi {
namespace loop {
class EventLoop;
} // namespace loop

namespace net {
class InetAddress;
class Connector;

using ConnectorPtr = std::shared_ptr<Connector>;

void RemoveConnection(loop::EventLoop* loop, const TcpConnectionPtr& conn);
void RemoveConnector(const ConnectorPtr& connector);

class TcpClient {
private:
    loop::EventLoop* loop_;
    ConnectorPtr connector_;
    TcpConnectionPtr connection_;
    const std::string name_;
    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    WriteCompleteCallback write_complete_callback_;
    bool retry_;
    bool connect_;
    int next_conn_id_;
    std::mutex mtx_;
    // void RemoveConnection(loop::EventLoop* loop, const TcpConnectionPtr& conn);
    // void RemoveConnector(const ConnectorPtr& connector);
    void NewConnection(int sockfd);
    void RemoveConnection(const TcpConnectionPtr& conn);

public:
    TcpClient(loop::EventLoop* loop, const InetAddress& serv_addr,
              const std::string& cli_name);
    void Connect();
    void Disconnect();
    void Stop();
    void EnableRetry();
    void SetMessageCallback(const MessageCallback& func);
    void SetWriteCompleteCallback(const WriteCompleteCallback& func);
    void SetConnectionCallback(const ConnectionCallback& func);
    TcpConnectionPtr Connection();
    loop::EventLoop* GetLoop() const;
    const std::string& GetName() const;
    bool GetRetry() const;
    ~TcpClient();
};

} // namespace net
} // namespace wasi

#endif