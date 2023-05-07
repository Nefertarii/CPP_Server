#include "Include/Net/tcpserver.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/eventloopthreadpool.h"
#include "Include/Net/socketpack.h"
#include "Include/Net/acceptor.h"
#include "Include/Logger/logger.h"

using namespace wasi;
using namespace wasi::net;

void TcpServer::NewConnection(int sockfd, const InetAddress& peer_addr) {
    if (loop_->IsInLoopThread()) {
        std::string conn_name = name_ + "-#" + std::to_string(next_conn_id_++);
        InetAddress peer_addr(SocketPack::GetPeerAddr(sockfd));
        LOG_INFO("new connection from:" + peer_addr.GetIpPort());
        InetAddress localaddr(SocketPack::GetLocalAddr(sockfd));
        TcpConnectionPtr conn = std::make_shared<TcpConnection>(
            loop_, conn_name, sockfd, localaddr, peer_addr);
        conntions_[conn_name] = conn;
        conn->SetConnectionCallback(connection_callback_);
        conn->SetMessageCallback(read_callback_);
        conn->SetWriteCompleteCallback(write_callback_);
        conn->SetCloseCallback(std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
        loop_->RunInLoop(std::bind(&TcpConnection::ConnectEstablished, conn));
    }
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& conn) {
    loop_->RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, conn));
}

void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& conn) {
    if (loop_->IsInLoopThread()) {
        if (conntions_.erase(conn->GetName()) == 1) {
            loop::EventLoop* io_loop = conn->GetLoop();
            io_loop->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
            LOG_INFO("Remove connection :" + conn->GetPeerAddress().GetIpPort());
        }
    }
}

TcpServer::TcpServer(loop::EventLoop* loop, const InetAddress& listen_addr,
                     const std::string& name, OptReusePort opt) :
    name_(name),
    ip_port_(listen_addr.GetIpPort()),
    loop_(loop), // acceptor loop;
    thread_pool_(std::make_shared<loop::EventLoopThreadPool>(loop_, name)), // process loop
    acceptor_(std::make_unique<Acceptor>(loop_, listen_addr)),
    connection_callback_(nullptr),
    read_callback_(nullptr),
    write_callback_(nullptr),
    thread_init_callback_(nullptr),
    started_(0),
    conntions_(),
    next_conn_id_(1) {
    acceptor_->SetNewConnectionCallback(std::bind(
        &TcpServer::NewConnection, this, std::placeholders::_1, std::placeholders::_2));
}

const std::string& TcpServer::GetIpPort() { return ip_port_; }

const std::string& TcpServer::GetName() { return name_; }

loop::EventLoop* TcpServer::GetLoop() { return loop_; }

std::shared_ptr<loop::EventLoopThreadPool> TcpServer::GetThreadPool() { return thread_pool_; }

void TcpServer::SetConnectionCallback(const ConnectionCallback& func) {
    connection_callback_ = func;
}

void TcpServer::SetMessageCallback(const MessageCallback& func) {
    read_callback_ = func;
}

void TcpServer::SetWriteCompleteCallback(const WriteCompleteCallback& func) {
    write_callback_ = func;
}

void TcpServer::SetThreadInitCallback(const ThreadInitCallback& func) {
    thread_init_callback_ = func;
}

void TcpServer::SetThreadNum(int num) {
    thread_pool_->SetThreadNum(num);
}

void TcpServer::Start() {
    if (started_.fetch_add(1) == 0) {
        //  thread_pool_->Start(thread_init_callback_);
        if (!acceptor_->Listening()) {
            loop_->RunInLoop(std::bind(&Acceptor::Listen, acceptor_.get()));
        }
    }
}

TcpServer::~TcpServer() {
    loop_->IsInLoopThread();
    {
        for (auto& conntions_it : conntions_) {
            TcpConnectionPtr conn(conntions_it.second);
            conntions_it.second.reset();
            conn->GetLoop()->RunInLoop(std::bind(
                &TcpConnection::ConnectDestroyed, conn));
        }
    }
}