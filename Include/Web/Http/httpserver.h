#ifndef WEB_HTTP_HTTPSERVER_H_
#define WEB_HTTP_HTTPSERVER_H_

#include "Include/Web/Http/httpcontext.h"
#include "Include/Web/Http/httpprocess.h"
#include "Include/Net/tcpserver.h"

namespace wasi {
namespace web {

using ParseCompleteCallBack = std::function<void()>;

class HttpServer {
private:
    std::shared_ptr<net::TcpServer> listen_server_;
    std::shared_ptr<loop::EventLoopThreadPool> thread_pool_;
    int fail_repeat_num_;
    int fail_repeat_time_;
    void SendRespone(const net::TcpConnectionPtr& conn);
    // new connection use
    void Connection(const net::TcpConnectionPtr& conn);
    // get message use
    void Message(const net::TcpConnectionPtr& conn);
    // write down use
    void WriteComplete(const net::TcpConnectionPtr& conn);
public:
    HttpServer(loop::EventLoop* loop,
               std::map<std::string, std::string> setting);
    void Start();
    void SetThreadNum(int num);
};

}
} // namespace wasi::web

#endif