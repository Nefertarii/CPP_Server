#ifndef SERVER_TCPSERVER_H_
#define SERVER_TCPSERVER_H_

#include "../../Base/Net/acceptor.h"
#include "../../Base/Net/inetaddress.h"
#include "../../Base/noncopyable.h"
#include "tcpconnection.h"
#include <atomic>
#include <map>
#include <memory>

namespace Wasi {
namespace Poll {

class EventLoop;
class EventLoopThreadPool;

} // namespace Poll

namespace Server {

using ConnectionMap      = std::map<std::string, TcpConnectionPtr>;
using ThreadInitCallback = std::function<void(Poll::EventLoop*)>;

class TcpServer : Noncopyable {
private:
    void New_connection(int sockfd, const Sockets::InetAddress& peeraddr);
    void Remove_connection(const TcpConnectionPtr& conn);
    void Remove_connection_in_loop(const TcpConnectionPtr& conn);
    const std::string name;
    const std::string ip_port;
    Poll::EventLoop* loop;                                  // acceptor loop;
    std::shared_ptr<Poll::EventLoopThreadPool> thread_pool; // process loop
    std::unique_ptr<Sockets::Acceptor> acceptor;
    ConnectionCallback connection_callback;
    MessageCallback read_callback;
    WriteCompleteCallback write_callback;
    ThreadInitCallback thread_init_callback;
    std::atomic<int> started;
    ConnectionMap conntions;
    int next_conn_id;

public:
    enum OptReusePort {
        NOREUSEPORT,
        REUSEPORT
    };
    TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr,
              const std::string& name_, OptReusePort opt);
    const std::string& Get_ip_port();
    const std::string& Get_name();
    Poll::EventLoop* Get_loop();
    std::shared_ptr<Poll::EventLoopThreadPool> Get_thread_pool();
    //当accept接受到一个新的连接产生一个新的文件描述符而创造了一个通信套接字的时进行回调
    void Set_connection_callback(const ConnectionCallback& callback_);
    //当通信套接字可读时进行回调 (epoll)
    void Set_message_callback(const MessageCallback& callback_);
    //当通信套接字可写时进行回调 (epoll)
    void Set_write_complete_callback(const WriteCompleteCallback& callback_);
    void Set_thread_init_callback(const ThreadInitCallback& callback);
    void Set_thread_num(int num);
    void Start();
    ~TcpServer();
};

} // namespace Server
} // namespace Wasi

#endif // !SERVER_TCPSERVER_H_