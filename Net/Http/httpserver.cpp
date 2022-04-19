#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../Poll/Head/eventloop.h"
#include "../Poll/Head/eventloopthreadpool.h"
#include "../Sockets/Head/socketapi.h"
#include "Head/httprequest.h"
#include "Head/httprespone.h"

using namespace Wasi::Http;
using namespace Wasi;

// void Parse_request(std::string http_request) {}
// void Process_request() {}
// void Prepare_respone() {}
// void Send_respone() {}

void Request_process(const Server::TcpConnectionPtr& conn) {
    // parse_request
    // std::shared_ptr<HttpContext> context = std::static_pointer_cast<HttpContext>(conn->Get_data_pointer());
    HttpContext* context                 = (HttpContext*)conn->Get_data_pointer().get();
    std::shared_ptr<HttpRequest> request = context->Get_request();
    std::shared_ptr<HttpRespone> respone = context->Get_respone();
    // process_request

    // prepare_respone

    // send_respone
    conn->Send("process done");
}

void HttpServer::Connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        LOG_INFO("Get connection");
        // use reserve and index can set max client;
        std::shared_ptr<HttpContext> new_conn_context = std::make_shared<HttpContext>();
        contexts.push_back(new_conn_context);
        std::shared_ptr<HttpContext> data_ptr = std::static_pointer_cast<HttpContext>(conn->Get_data_pointer());
        data_ptr                              = new_conn_context;
        // HttpContext* context_ptr              = data_ptr.get();
        // context_ptr                           = new_conn_context.get();
        conn->Send("Get connection");
    }
}

void HttpServer::Message(const Server::TcpConnectionPtr& conn) {
    Base::Buffer* read                = conn->Get_input_buffer();
    std::string msg                   = "From " + conn->Get_peer_address().To_string_ip_port() + " get message: " + read->Content();
    Poll::EventLoop* thread_pool_loop = thread_pool->Get_loop();
    thread_pool_loop->Run_in_loop(std::bind(&Request_process, conn));
    LOG_INFO(msg);
}

void HttpServer::Write_complete(const Server::TcpConnectionPtr& conn) {
    std::string msg = "Write: write_complete";
    LOG_INFO(msg);
    conn->Send(msg);
}

HttpServer::HttpServer(Poll::EventLoop* loop,
                       const Sockets::InetAddress& listen_addr,
                       const std::string& name,
                       Server::TcpServer::OptReusePort option) :
    listen_server(loop, listen_addr, name, option),
    thread_pool(listen_server.Get_thread_pool()) {
    listen_server.Set_connection_callback(std::bind(
        &HttpServer::Connection, this, std::placeholders::_1));
    listen_server.Set_message_callback(std::bind(
        &HttpServer::Message, this, std::placeholders::_1));
}

void HttpServer::Start() {
    std::string msg = "HttpServer [" + listen_server.Get_name()
                      + "] starts listening on " + listen_server.Get_ip_port();
    LOG_INFO(msg);
    thread_pool->Set_thread_num(std::thread::hardware_concurrency());
    listen_server.Start();
    thread_pool->Start();
}

void HttpServer::Set_thread_num(int num) {
    thread_pool->Set_thread_num(num);
}