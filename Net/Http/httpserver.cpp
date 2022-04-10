#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socketapi.h"

using namespace Wasi::Http;
using namespace Wasi;

void HttpServer::connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        LOG_INFO("Get connection");
        conn->Send("Get connection\n");
    }
}

void HttpServer::message(const Server::TcpConnectionPtr& conn) {
    Base::Buffer* read = conn->Get_input_buffer();
    std::string msg    = "Get: " + read->Content();
    LOG_INFO(msg);
    conn->Send(msg);
}

void HttpServer::write_complete(const Server::TcpConnectionPtr& conn) {
    std::string msg = "Write: write_complete";
    LOG_INFO(msg);
    conn->Send(msg);
}

void HttpServer::parse_request() {}

void HttpServer::process_request() {}

void HttpServer::send_respone() {}

HttpServer::HttpServer(Poll::EventLoop* loop,
                       const Sockets::InetAddress& listen_addr,
                       const std::string& name,
                       Server::TcpServer::OptReusePort option) :
    listen_server(loop, listen_addr, name, option),
    threadpool(),
    contexts() {
    listen_server.Set_connection_callback(std::bind(
        &HttpServer::connection, this, std::placeholders::_1));
    listen_server.Set_message_callback(std::bind(
        &HttpServer::message, this, std::placeholders::_1));
    // listen_server.Set_write_complete_callback(std::bind(
    //     &HttpServer::write_complete, this, std::placeholders::_1));
}

void HttpServer::Start() {
    std::string msg = "HttpServer [" + listen_server.Get_name()
                      + "] starts listening on " + listen_server.Get_ip_port();
    LOG_INFO(msg);
    // threadpool.Start();
    listen_server.Start();
}

void HttpServer::Set_thread_num(int num) {
    threadpool.Set_thread_count(num);
}