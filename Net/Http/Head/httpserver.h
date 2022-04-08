#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "../../../Thread/Head/threadpool.h"
#include "../../Server/Head/tcpserver.h"

namespace Wasi {
namespace Http {

class HttpRequest;
class HttpRespone;

using HttpCallback = std::function<void(const HttpRequest&, HttpRespone*)>;

class HttpServer : Noncopyable {
private:
    Poll::EventLoop* base_loop;
    int next_conn_id;
    int num_thread;
    std::atomic<int> started;
    const std::string name;
    const std::string ip_port;
    std::vector<std::unique_ptr<Poll::EventLoop>> loops;
    std::unique_ptr<Sockets::Acceptor> acceptor;
    ConnectionMap conntions;
    Server::ConnectionCallback connection_callback;
    Server::MessageCallback read_callback;
    Server::WriteCompleteCallback write_callback;
    Base::ThreadPool thread_pool;

public:
    HttpServer(Poll::EventLoop* loop, int num_thread_ = 0,
               const Sockets::InetAddress listenaddr,
               const std::string& name_, bool reuse_port);
    Poll::EventLoop* Get_loop() const;
    void Set_connection_callback(const ConnectionCallback& callback_);
    void Set_read_callback(const MessageCallback& callback_);
    void Set_write_callback(const WriteCompleteCallback& callback_);
    void Remove_connection(const Server::TcpConnectionPtr& conn);
    void Set_thread_num(int num);
    void Start();
};

}
} // namespace Wasi::Http

#endif // !HTTP_SERVER_H_