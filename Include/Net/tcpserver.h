#ifndef NET_TCPSERVER_H_
#define NET_TCPSERVER_H_

#include "Include/Net/tcpconnection.h"
#include <map>
#include <atomic>

namespace wasi {
namespace loop {
class EventLoop;
class EventLoopThreadPool;
} // namespace loop

namespace net {
class InetAddress;
class Acceptor;

using ConnectionMap      = std::map<std::string, TcpConnectionPtr>;
using ThreadInitCallback = std::function<void(loop::EventLoop*)>;

enum OptReusePort {
    kNoreuseport,
    kReuseport
};

class TcpServer {
private:
    std::string name_;
    std::string ip_port_;
    loop::EventLoop* loop_;                                  // acceptor loop;
    std::shared_ptr<loop::EventLoopThreadPool> thread_pool_; // process loop
    std::unique_ptr<Acceptor> acceptor_;
    ConnectionCallback connection_callback_;
    MessageCallback read_callback_;
    WriteCompleteCallback write_callback_;
    ThreadInitCallback thread_init_callback_;
    std::atomic<int> started_;
    ConnectionMap conntions_;
    int next_conn_id_;
    void NewConnection(int sockfd, const InetAddress& peer_addr);
    void RemoveConnection(const TcpConnectionPtr& conn);
    void RemoveConnectionInLoop(const TcpConnectionPtr& conn);

public:
    TcpServer(loop::EventLoop* loop_, const InetAddress& listen_addr,
              const std::string& name_, OptReusePort opt = kNoreuseport);
    const std::string& GetIpPort();
    const std::string& GetName();
    loop::EventLoop* GetLoop();
    std::shared_ptr<loop::EventLoopThreadPool> GetThreadPool();
    void SetConnectionCallback(const ConnectionCallback& func);
    void SetMessageCallback(const MessageCallback& func);
    void SetWriteCompleteCallback(const WriteCompleteCallback& func);
    void SetThreadInitCallback(const ThreadInitCallback& func);
    void SetThreadNum(int num);
    void Start();
    ~TcpServer();
};

} // namespace net
} // namespace wasi

#endif