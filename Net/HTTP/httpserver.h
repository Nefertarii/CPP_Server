#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "../../../Log/Head/logger.h"
#include "../../../Thread/Head/threadpool.h"
#include "../../Server/Head/tcpserver.h"
#include "httpcontext.h"
#include <vector>

namespace Wasi {
namespace Http {

// using HttpCallback = std::function<void(const HttpRequest&, HttpRespone*)>;
using ParseCompleteCallBack = std::function<void()>;

class HttpServer : Noncopyable {
private:
    Server::TcpServer listen_server;
    // Server::TcpServer process_server;
    std::shared_ptr<Poll::EventLoopThreadPool> thread_pool;
    std::vector<HttpContext> contexts;

    void Connection(const Server::TcpConnectionPtr& conn);
    void Message(const Server::TcpConnectionPtr& conn);
    void Write_complete(const Server::TcpConnectionPtr& conn);

public:
    HttpServer(Poll::EventLoop* loop,
               const Sockets::InetAddress& listen_addr,
               const std::string& name,
               Server::TcpServer::OptReusePort option = Server::TcpServer::OptReusePort::NOREUSEPORT);
    void Start();
    void Set_thread_num(int num);
};

}
} // namespace Wasi::Http

#endif // !HTTP_SERVER_H_