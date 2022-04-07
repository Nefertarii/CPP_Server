#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "Head/httprequest.h"
#include "Head/httprespone.h"

using namespace Wasi::Http;
using namespace Wasi;

void HttpServer::Connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        Base::Buffer read = conn.Get_input_buffer();
    } else {
    }
}

void HttpServer::Message(const Server::TcpConnectionPtr& conn) {
}

void HttpServer::Request(const Server::TcpConnectionPtr& conn) {
}

HttpServer::HttpServer(Poll::EventLoop* loop,
                       const Sockets::InetAddress& listen_addr,
                       const std::string& name,
                       Server::TcpServer::OptReusePort option) {
    server.Set_connection_callback(std::bind(&HttpServer::Connection, this, std::placeholders::_1));
    server.Set_message_callback(&HttpServer::Message, this, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3);
}

Poll::EventLoop* HttpServer::Get_loop() const { return server.Get_loop(); }

void HttpServer::Set_http_callback(const HttpCallback& callback) {
    http_callback = callback;
}

void HttpServer::Set_thread_num(int num) {}

void HttpServer::Start() {
    std::string msg = "HttpServer[" + server.Get_name()
                      + "] starts listening on " + server.Get_ip_port();
    LOG_INFO(msg);
    server.Start();
}