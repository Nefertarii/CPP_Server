#include "Include/Net/tcpclient.h"
#include "Include/Net/inetaddress.h"
#include "Include/Net/socketpack.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Net/connector.h"
#include "Include/Logger/logger.h"

using namespace wasi;
using namespace wasi::net;

void net::RemoveConnection(loop::EventLoop* loop, const TcpConnectionPtr& conn) {
    loop->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
}

void net::RemoveConnector(const ConnectorPtr& connector) {
}

void TcpClient::NewConnection(int sockfd) {
    if (loop_->IsInLoopThread()) {
        InetAddress peer_addr(SocketPack::GetPeerAddr(sockfd));
        std::string conn_info;
        conn_info = ":" + peer_addr.GetIpPort()
                    + "-#" + std::to_string(next_conn_id_);
        std::string conn_name = name_ + conn_info;
        InetAddress local_addr(SocketPack::GetLocalAddr(sockfd));
        TcpConnectionPtr conn(new TcpConnection(loop_, conn_name, sockfd,
                                                local_addr, peer_addr));
        conn->SetConnectionCallback(connection_callback_);
        conn->SetMessageCallback(message_callback_);
        conn->SetWriteCompleteCallback(write_complete_callback_);
        conn->SetCloseCallback(std::bind(&TcpClient::RemoveConnection, this, std::placeholders::_1));
        {
            std::lock_guard<std::mutex> lk(mtx_);
            connection_ = conn;
        }
        conn->ConnectEstablished();
    }
}

void TcpClient::RemoveConnection(const TcpConnectionPtr& conn) {
    if (loop_->IsInLoopThread()) {
        if (loop_ == conn->GetLoop()) {
            {
                std::lock_guard<std::mutex> lk(mtx_);
                if (connection_ == conn) {
                    connection_.reset();
                }
            }
            loop_->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
            if (retry_ && connect_) {
                LOG_INFO(name_ + " reconnecting to" + connector_->GetPeerAddr().GetIpPort());
                connector_->Restart();
            }
        }
    }
}

TcpClient::TcpClient(loop::EventLoop* loop, const InetAddress& serv_addr,
                     const std::string& cli_name) :
    loop_(loop),
    connector_(std::make_shared<Connector>(loop, serv_addr)),
    connection_(nullptr),
    name_(cli_name),
    connection_callback_(nullptr),
    message_callback_(nullptr),
    write_complete_callback_(nullptr),
    retry_(false),
    connect_(true),
    next_conn_id_(1),
    mtx_() {
    connector_->SetNewConnectionCallback(
        std::bind(&TcpClient::NewConnection, this, std::placeholders::_1));
    // const void* address = static_cast<const void*>(this);
    // std::stringstream point;
    // point << address;
    // std::string msg = "TcpClient::TcpClient[" + point.str() + "] connector ";
    // address         = static_cast<const void*>(connector.get());
    // point << address;
    // msg += point.str();
    // LOG_INFO(msg);
}

void TcpClient::Connect() {
    LOG_INFO(name_ + " connecting to:" + connector_->GetPeerAddr().GetIpPort());
    connect_ = true;
    connector_->Start();
}

void TcpClient::Disconnect() {
    connect_ = false;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        if (connection_) { connection_->Shutdown(); }
    }
}

void TcpClient::Stop() {
    connect_ = false;
    connector_->Stop();
}

void TcpClient::EnableRetry() { retry_ = true; }

void TcpClient::SetMessageCallback(const MessageCallback& func) {
    message_callback_ = func;
}

void TcpClient::SetWriteCompleteCallback(const WriteCompleteCallback& func) {
    write_complete_callback_ = func;
}

void TcpClient::SetConnectionCallback(const ConnectionCallback& func) {
    connection_callback_ = func;
}

TcpConnectionPtr TcpClient::Connection() {
    std::lock_guard<std::mutex> lk(mtx_);
    return connection_;
}

loop::EventLoop* TcpClient::GetLoop() const { return loop_; }

const std::string& TcpClient::GetName() const { return name_; }

bool TcpClient::GetRetry() const { return retry_; }

TcpClient::~TcpClient() {
    // const void* address = static_cast<const void*>(connector_.get());
    // std::stringstream point;
    // point << address;
    // std::string msg = "TcpClient::~TcpClient [" + name + "] connector " + point.str();
    // LOG_DEBUG(msg);
    TcpConnectionPtr conn;
    bool unique = false;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        unique = connection_.unique();
        conn   = connection_;
    }
    if (conn) {
        if (loop_ == conn->GetLoop()) {
            CloseCallback close_callback = std::bind(&net::RemoveConnection, loop_, std::placeholders::_1);
            loop_->RunInLoop(std::bind(&TcpConnection::SetCloseCallback, conn, close_callback));
            if (unique) { conn->ForceClose(); }
        }
    } else {
        connector_->Stop();
        loop_->RunAfter(1.0, std::bind(&net::RemoveConnector, connector_));
    }
}