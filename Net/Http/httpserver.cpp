#include "httpserver.h"
#include "httpprocess.h"
#include "httprequest.h"
#include "httprespone.h"
#include <Base/Net/socketapi.h>
#include <Base/Poll/eventloop.h>
#include <Base/Poll/eventloopthreadpool.h>
#include <Base/Timer/clock.h>
#include <Base/filestat.h>
#include <Log/logging.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>

using namespace Wasi::Http;
using namespace Wasi;

void HttpServer::Connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        LOG_INFO("Get connection");
        // use reserve and index can set max client;
        HttpContext new_conn_context;
        contexts.push_back(new_conn_context);
        conn->Set_context(new_conn_context);
    }
}

void HttpServer::Message(const Server::TcpConnectionPtr& conn) {
    Base::Buffer* read                = conn->Get_input_buffer();
    std::string msg                   = "From " + conn->Get_peer_address().To_string_ip_port() + " get message:\n" + read->Content();
    Poll::EventLoop* thread_pool_loop = thread_pool->Get_loop();
    thread_pool_loop->Run_in_loop(std::bind(&Request_process, conn));
    LOG_DEBUG(msg);
}

void HttpServer::Write_complete(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    Base::FileStat* conn_file_stat            = conn->Get_file_stat();
    // head write complete
    // write body
    if (!conn_respone->respone_body.empty()) {
        conn->Send(conn_respone->respone_body);
        conn_respone->Init();
        return;
    } else if (!conn_respone->respone_file.empty()) {
        conn->Sendfile(conn_respone->respone_file);
        conn_respone->Init();
    } else {
        if (conn_file_stat->filefd == 0) { return; }
        if (conn_file_stat->Remaning() == 0) { return; }
        if (!conn_respone->respone_file.empty()) {
            conn->Sendfile(conn_file_stat->file_name);
            conn_respone->respone_file.clear();
        }
    }
    // write file
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
    listen_server.Set_write_complete_callback(std::bind(
        &HttpServer::Write_complete, this, std::placeholders::_1));
}

void HttpServer::Start() {
    std::string msg = "HttpServer [" + listen_server.Get_name()
                      + "] starts listening on " + listen_server.Get_ip_port();
    thread_pool->Set_thread_num(std::thread::hardware_concurrency());
    listen_server.Start();
    thread_pool->Start();
    LOG_INFO(msg);
}

void HttpServer::Set_thread_num(int num) {
    thread_pool->Set_thread_num(num);
}