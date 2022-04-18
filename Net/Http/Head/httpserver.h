#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "../../../Thread/Head/threadpool.h"
#include "../../Server/Head/tcpserver.h"
#include "httpcontext.h"
#include <vector>

namespace Wasi {
namespace Http {

// using HttpCallback = std::function<void(const HttpRequest&, HttpRespone*)>;

class HttpServer : Noncopyable {
private:
    Server::TcpServer listen_server;
    // Server::TcpServer process_server;
    Base::ThreadPool threadpool;
    //std::vector<HttpContext> contexts;
    void connection(const Server::TcpConnectionPtr& conn);
    void message(const Server::TcpConnectionPtr& conn);
    void write_complete(const Server::TcpConnectionPtr& conn);

    void parse_request();
    void process_request();
    void send_respone();

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