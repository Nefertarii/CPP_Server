#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../../Timer/Head/clock.h"
#include "../Poll/Head/eventloop.h"
#include "../Poll/Head/eventloopthreadpool.h"
#include "../Sockets/Head/socketapi.h"
#include "Head/httprequest.h"
#include "Head/httprespone.h"

using namespace Wasi::Http;
using namespace Wasi;

int Parse_request(const Server::TcpConnectionPtr& conn) {
    // Preaccess phase: IP control(black list)
    // ...
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    conn_request->Parse(conn->Get_input_buffer()->Content());
}

int Get_process(const Server::TcpConnectionPtr& conn) {
}

int Post_process(const Server::TcpConnectionPtr& conn) {
    // Access phase: Access control(file)
    // fill HttpRequest body
    // fill HttpRequest
}

int Process_request(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    if (conn_request->Get_method() == Method::GET) {
        // Get_process();
    } else if (conn_request->Get_method() == Method::POST) {
        // Post_process();
    }
    return -1;
}

int Prepare_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    // conn_respone->Set_code_num(conn_request->Get_code_num());
    if (conn_request->Get_version() == Version::HTTP11) {
        conn_respone->Set_connection_type("keep-alive");
    }
    conn_respone->Set_content_type("UTF-8");
    conn_respone->Set_server_name("HttpServer");
    int content_length = conn_request->Get_body().length();
    conn_respone->Set_content_length(std::to_string(content_length));
    // conn_respone->Set_last_modified(file_modify_time);
}

int Send_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    // conn->Send(conn_respone->Get_respone_head());
    conn->Send(conn_respone->Get_respone_body());
}

void Request_process(const Server::TcpConnectionPtr& conn) {
    std::string msg;
    // parse_request
    Parse_request(conn);
    // process_request
    Process_request(conn);
    // prepare_respone
    Prepare_respone(conn);
    // send_respone
    Send_respone(conn);
    LOG_INFO(msg);
}

void HttpServer::Connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        LOG_INFO("Get connection");
        // use reserve and index can set max client;
        HttpContext new_conn_context;
        contexts.push_back(new_conn_context);
        conn->Set_context(new_conn_context);
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