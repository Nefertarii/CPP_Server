#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socketapi.h"

using namespace Wasi::Http;
using namespace Wasi;

void HttpServer::New_connection(int sockfd, const Sockets::InetAddress& peeraddr) {
    base_loop->Assert_in_loop_thread();
    std::string conn_name = name + "#" + std::to_string(next_conn_id);
    ++next_conn_id;
    std::string msg = "HttpServer::New_connection [" + name
                      + "] new connection [" + conn_name
                      + "] from" + peeraddr.To_string_ip_port();
    LOG_INFO(msg);
    Sockets::InetAddress localaddr(Sockets::Get_local_addr(sockfd));
    Server::TcpConnectionPtr conn_ptr =
        std::make_shared<Server::TcpConnection>(base_loop, conn_name, sockfd, localaddr, peeraddr);
    msg = conn_ptr->Get_local_address().To_string_ip_port() + " -> "
          + conn_ptr->Get_peer_address().To_string_ip_port() + " is ";
    msg += conn_ptr->Connected() ? "Up" : "Down";
    LOG_INFO(msg);
    conntions[conn_name] = conn_ptr;
    conn_ptr->Set_connection_callback(connection_callback);
    conn_ptr->Set_message_callback(read_callback);
    conn_ptr->Set_write_complete_callback(write_callback);
    conn_ptr->Set_close_callback(std::bind(&HttpServer::Remove_connection, this, std::placeholders::_1));
    base_loop->Run_in_loop(std::bind(&Server::TcpConnection::Connect_established, conn));
}

void HttpServer::Remove_connection_inloop(const Server::TcpConnectionPtr& conn) {
}

HttpServer::HttpServer(Poll::EventLoop* loop, int num_thread_,
                       const Sockets::InetAddress listenaddr,
                       const std::string& name_, bool reuse_port) :
    base_loop(loop),
    next_conn_id(0),
    num_thread(num_thread_),
    started(0),
    name(name_),
    ip_port(),
    acceptor(std::make_unique<Sockets::Acceptor>(base_loop, listenaddr, reuse_port)),
    conntions(),
    connection_callback(nullptr),
    read_callback(nullptr),
    write_callback(nullptr),
    thread_pool() {
    acceptor->Set_new_connection_callback(
        std::bind(&HttpServer::New_connection, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
    for (int i = 0; i < num_thread; ++i) {
        loops.push_back(std::make_unique<Poll::EventLoop>());
    }
}

Poll::EventLoop* HttpServer::Get_loop() const { return base_loop; }

void HttpServer::Set_connection_callback(const ConnectionCallback& callback_) {
    connection_callback = callback_;
}

void HttpServer::Set_read_callback(const MessageCallback& callback_) {
    read_callback = callback_;
}

void HttpServer::Set_write_callback(const WriteCompleteCallback& callback_) {
    write_callback = callback_;
}

void HttpServer::Remove_connection(const Server::TcpConnectionPtr& conn) {
    Remove_connection_inloop(conn);
}

void HttpServer::Set_thread_num(int num) { num_thread = num; }

void HttpServer::Start() {
    if (started.fetch_add(1) == 0) {
        assert(!acceptor->Listening());
        loop->Run_in_loop(std::bind(&Sockets::Acceptor::Listen, acceptor.get()));
    }
}
