#include "tcpserver.h"
#include <Base/Net/socketapi.h>
#include <Base/eventloop.h>
#include <Base/eventloopthreadpool.h>
#include <Log/logging.h>
#include <cassert>

using namespace Wasi;
using namespace Wasi::Server;

void TcpServer::New_connection(int sockfd, const Sockets::InetAddress& peeraddr) {
    loop->Assert_in_loop_thread();
    std::string conn_name = name + "-#" + std::to_string(next_conn_id);
    ++next_conn_id;
    std::string msg = "New_connection " + name
                      + "] new connection [" + conn_name
                      + "] from" + peeraddr.To_string_ip_port();
    LOG_INFO(msg);
    Sockets::InetAddress localaddr(Sockets::Get_local_addr(sockfd));
    TcpConnectionPtr conn = std::make_shared<TcpConnection>(
        loop, conn_name, sockfd, localaddr, peeraddr);
    conntions[conn_name] = conn;
    conn->Set_connection_callback(connection_callback);
    conn->Set_message_callback(read_callback);
    conn->Set_write_complete_callback(write_callback);
    conn->Set_close_callback(std::bind(&TcpServer::Remove_connection, this, std::placeholders::_1));
    loop->Run_in_loop(std::bind(&TcpConnection::Connect_established, conn));
    // conn->Connect_established();
}

void TcpServer::Remove_connection(const TcpConnectionPtr& conn) {
    loop->Run_in_loop(std::bind(&TcpServer::Remove_connection_in_loop, this, conn));
}

void TcpServer::Remove_connection_in_loop(const TcpConnectionPtr& conn) {
    loop->Assert_in_loop_thread();
    std::string msg = "Remove [" + name
                      + "] connection " + conn->Get_name();
    LOG_INFO(msg);
    size_t earse = conntions.erase(conn->Get_name());
    assert(earse == 1);
    Poll::EventLoop* io_loop = conn->Get_loop();
    io_loop->Queue_in_loop(std::bind(&TcpConnection::Connect_destroyed, conn));
}

TcpServer::TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr,
                     const std::string& name_, OptReusePort opt) :
    name(name_),
    ip_port(listenaddr.To_string_ip_port()),
    loop(loop_),
    thread_pool(std::make_shared<Poll::EventLoopThreadPool>(loop, name)),
    acceptor(std::make_unique<Sockets::Acceptor>(loop, listenaddr, REUSEPORT)),
    connection_callback(),
    read_callback(),
    write_callback(),
    started(0),
    conntions(),
    next_conn_id(1) {
    acceptor->Set_new_connection_callback(std::bind(
        &TcpServer::New_connection, this, std::placeholders::_1, std::placeholders::_2));
}

const std::string& TcpServer::Get_ip_port() { return ip_port; }

const std::string& TcpServer::Get_name() { return name; }

Poll::EventLoop* TcpServer::Get_loop() { return loop; }

std::shared_ptr<Poll::EventLoopThreadPool> TcpServer::Get_thread_pool() {
    return thread_pool;
}

void TcpServer::Set_connection_callback(const ConnectionCallback& callback_) {
    connection_callback = callback_;
}

void TcpServer::Set_message_callback(const MessageCallback& callback_) {
    read_callback = callback_;
}

void TcpServer::Set_write_complete_callback(const WriteCompleteCallback& callback_) {
    write_callback = callback_;
}

void TcpServer::Set_thread_num(int num) {
    if (num <= 0) {
        LOG_ERROR("thread num set:" + std::to_string(num));
        return;
    }
    thread_pool->Set_thread_num(num);
}

void TcpServer::Set_thread_init_callback(const ThreadInitCallback& callback_) {
    thread_init_callback = callback_;
}

void TcpServer::Start() {
    if (started.fetch_add(1) == 0) {
        // thread_pool->Start(thread_init_callback);
        assert(!acceptor->Listening());
        loop->Run_in_loop(std::bind(&Sockets::Acceptor::Listen, acceptor.get()));
    }
}

TcpServer::~TcpServer() {
    loop->Assert_in_loop_thread();
    LOG_DEBUG("TcpServer [" + name + "] destructing.")
    for (auto& i : conntions) {
        TcpConnectionPtr conn(i.second);
        i.second.reset();
        conn->Get_loop()->Run_in_loop(std::bind(
            &TcpConnection::Connect_destroyed, conn));
    }
}