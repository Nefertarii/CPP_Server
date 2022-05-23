#ifndef SERVER_TCPCLIENT_H_
#define SERVER_TCPCLIENT_H_

#include <Base/noncopyable.h>
#include "tcpconnection.h"
#include <memory>
#include <mutex>
#include <string>

namespace Wasi {
namespace Poll {

class EventLoop;

} // namespace Poll
namespace Sockets {

class InetAddress;

class Connector;

} // namespace Sockets
namespace Server {

using ConnectorPtr = std::shared_ptr<Sockets::Connector>;

void Remove_connection(Poll::EventLoop* loop, const TcpConnectionPtr& conn);

void Remove_connector(const ConnectorPtr& connector);

class TcpClient : Noncopyable {
private:
    void New_connection(int sockfd);
    void Remove_connection(const TcpConnectionPtr& conn);
    Poll::EventLoop* loop;
    ConnectorPtr connector;
    TcpConnectionPtr connection;
    const std::string name;
    ConnectionCallback connection_callback;
    MessageCallback message_callback;
    WriteCompleteCallback write_complete_callback;
    bool retry;
    bool connect;
    int next_conn_id;
    std::mutex mtx;

public:
    TcpClient(Poll::EventLoop* loop_,
              const Sockets::InetAddress& serveraddr,
              const std::string& cli_name);
    void Connect();
    void Disconnect();
    void Stop();
    void Enable_retry();
    void Set_message_callback(const MessageCallback& callback);
    void Set_write_complete_callback(const WriteCompleteCallback& callback);
    void Set_connection_callback(const ConnectionCallback& callback);
    TcpConnectionPtr Connection();
    Poll::EventLoop* Get_loop() const;
    const std::string& Get_name() const;
    bool Get_retry() const;
    ~TcpClient();
};

} // namespace Server
} // namespace Wasi

#endif // !SERVER_TCPCLIENT_H_