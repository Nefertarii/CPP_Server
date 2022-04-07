#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "../../../Thread/Head/threadpool.h"
#include "../../Server/Head/tcpserver.h"

namespace Wasi {
namespace Poll {

class EventLoop;

}
namespace Http {

class HttpRequest;
class HttpRespone;

using HttpCallback = std::function<void(const HttpRequest&, HttpRespone*)>;

class HttpServer : Noncopyable {
private:
    Poll::EventLoop base_loop;
    Server::TcpServer listen_server;
    Server::TcpServer process_server;
    Base::ThreadPool thread_pool;
    std::vector<Server::TcpConnectionPtr> conntions;
    HttpCallback http_callback; // main process function
    void Connection(const Server::TcpConnectionPtr& conn);
    void Message(const Server::TcpConnectionPtr& conn);
    void Request(const Server::TcpConnectionPtr& conn);

public:
    HttpServer(Poll::EventLoop loop);
    Poll::EventLoop* Get_loop() const;
    void Set_http_callback(const HttpCallback& callback);
    void Set_thread_num(int num);
    void Start();
};

} // namespace Http
} // namespace Wasi

#endif // !HTTP_SERVER_H_