#include "Head/tcpserver.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socketapi.h"
#include <cassert>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Server;

void TcpServer::New_connection(int sockfd, const Sockets::InetAddress peeraddr) {
    loop->Assert_in_loop_thread();
    std::string conn_name = name + std::to_string(next_conn_id);
    ++next_conn_id;
    std::cout << "TcpServer::New_connection [" << name
        << "] new connection [" << conn_name
        << "] from" << peeraddr.To_string_ip_port() << "\n";
    Sockets::InetAddress localaddr(Sockets::Get_local_addr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop, conn_name, sockfd, localaddr, peeraddr));
    std::cout << conn->Get_local_address().To_string_ip_port()
        << " -> " << conn->Get_peer_address().To_string_ip_port()
        << " is " << (conn->Connected() ? "Up" : "Down");
    conntions[conn_name] = conn;
    conn->Set_connection_callback(connection_callback);
    conn->Set_message_callback(message_callback);
    conn->Connect_established();
}

void TcpServer::Remove_connection(const TcpConnectionPtr& conn) {}

void TcpServer::Remove_connection_in_loop(const TcpConnectionPtr& conn) {}

TcpServer::TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr,
                     const std::string& name_, OptReusePort opt) :
    loop(loop_),
    name(name_),
    acceptor(new Sockets::Acceptor(loop, listenaddr, (opt == REUSEPORT))),
    next_conn_id(1) {
    acceptor->Set_new_connection_callback(std::bind(&TcpServer::New_connection, this, std::placeholders::_1, std::placeholders::_2));
}

const std::string& TcpServer::Get_ip_port() { return ip_port; }

const std::string& TcpServer::Get_name() { return name; }

Poll::EventLoop* TcpServer::Get_loop() { return loop; }

//void TcpServer::Set_thread_num(int num_threads) {}

//void TcpServer::Set_thread_init_callback(const ThreadInitCallback& callback_) {}

void TcpServer::Set_connection_callback(const ConnectionCallback& callback_) {
    connection_callback = callback_;
}

void TcpServer::Set_message_callback(const MessageCallback& callback_) {
    message_callback = callback_;
}

void TcpServer::Set_write_callback(const WriteCompleteCallback& callback_) {
    write_complete_callback = callback_;
}

void TcpServer::Start() {
    if (started.fetch_add(1) == 0) {
        assert(!acceptor->Listening());
        started = true;
        //loop->Run_in_loop(std::bind(&Sockets::Acceptor::Listen, std::move(acceptor)));
        loop_->runInLoop(std::bind(&Sockets::Acceptor::Listen,  (acceptor_)));
    }
}

TcpServer::~TcpServer() {}