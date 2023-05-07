#include "Include/Net/connector.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/channel.h"
#include "Include/Logger/logger.h"
#include "Include/Net/socketpack.h"
#include <sstream>
#include <cassert>

using namespace wasi;
using namespace wasi::net;

const int Connector::max_retry_delay_  = 30 * 1000; // ms
const int Connector::init_retry_delay_ = 500;       // ms

void Connector::SetState(ConnectorState state) {   state_ = state;}

std::string Connector::StrState(ConnectorState state) {
    switch (state) {
    case kConnectordisconnected:
        return "disconnected";
    case kConnectorconnecting:
        return "connecting";
    case kConnectorconnected:
        return "connected";
    default:
        return "unkown state";
    }
}

void Connector::StartInLoop() {
    if (loop_->IsInLoopThread()) {
        if (state_ == kConnectordisconnected) {
            if (connect_) {
                Connect();
            } else {
                LOG_DEBUG(peer_addr_.GetIpPort() + " is connecting");
            }
        } else {
            LOG_DEBUG(peer_addr_.GetIpPort() + " is connected");
        }
    }
}

void Connector::StopInLoop() {
    loop_->IsInLoopThread();
    if (state_ == kConnectorconnecting) {
        SetState(kConnectordisconnected);
        int sockfd = RemoveAndReset();
        Retry(sockfd);
    }
}

void Connector::Connect() {
    int sockfd    = SocketPack::CreateSocket(peer_addr_.GetFamily());
    int conn_ret  = SocketPack::Connect(sockfd, peer_addr_.GetSockaddr());
    int tmp_errno = (conn_ret == 0) ? 0 : errno;
    if (tmp_errno > 0) {
        if (tmp_errno == EISCONN || tmp_errno == EINPROGRESS) {
            Connecting(sockfd);
        } else {
            LOG_ERROR("errno:" + std::to_string(tmp_errno));
            SocketPack::Close(sockfd);
        }
    }
}

void Connector::Connecting(int sockfd) {
    SetState(kConnectorconnecting);
    if (!channel_) {
        channel_.reset(new loop::Channel(loop_, sockfd));
        channel_->SetWriteCallback(std::bind(&Connector::HandleWrite, this));
        channel_->SetErrorCallback(std::bind(&Connector::HandleError, this));
        channel_->EnableWriting();
    }
}

void Connector::HandleWrite() {
    if (state_ == kConnectorconnecting) {
        int sockfd  = RemoveAndReset();
        int sockerr = SocketPack::GetSocketError(sockfd);
        if (sockerr) {
            LOG_ERROR("in connecting, errno:" + std::to_string(sockerr));
            Retry(sockfd);
        } else {
            SetState(kConnectorconnected);
            if (connect_) {
                new_connection_callback_(sockfd);
            } else {
                SocketPack::Close(sockfd);
            }
        }
    }
}

void Connector::HandleError() {
    if (state_ == kConnectorconnecting) {
        int sockfd  = RemoveAndReset();
        int sockerr = SocketPack::GetSocketError(sockfd);
        LOG_ERROR("errno: " + std::to_string(sockerr));
        Retry(sockfd);
    } else {
        LOG_ERROR("error in connected");
    }
}

void Connector::Retry(int sockfd) {
    SocketPack::Close(sockfd);
    SetState(kConnectordisconnected);
    if (connect_) {
        std::string msg = "retry connecting to " + peer_addr_.GetIpPort()
                          + " in " + std::to_string(retry_delay_) + " milliseconds.";
        LOG_DEBUG(msg);
        loop_->RunAfter(retry_delay_ / 1000.0,
                        std::bind(&Connector::StartInLoop, this));
        retry_delay_ = std::min(retry_delay_ * 2, max_retry_delay_);
    } else {
        LOG_ERROR("can't connect");
    }
}

void Connector::ResetChannel() { channel_.reset(); }

int Connector::RemoveAndReset() {
    channel_->DisableAll();
    int sockfd = channel_->GetFd();
    loop_->QueueInLoop(std::bind(&Connector::ResetChannel, this));
    return sockfd;
}

Connector::Connector(loop::EventLoop* loop, const InetAddress& peer_addr) :
    loop_(loop),
    peer_addr_(peer_addr),
    retry_delay_(init_retry_delay_),
    connect_(false),
    state_(kConnectordisconnected),
    channel_(),
    new_connection_callback_() {
    const void* this_address = static_cast<const void*>(this);
    std::stringstream str_this_address;
    str_this_address << this_address;
    LOG_DEBUG("Connector create in :" + str_this_address.str());
}

void Connector::SetNewConnectionCallback(const NewConnectionCallback& func) {
    new_connection_callback_ = func;
}

void Connector::Start() {
    connect_ = true;
    loop_->RunInLoop(std::bind(&Connector::StartInLoop, this));
}

void Connector::Restart() {
    Stop();
    Start();
}

void Connector::Stop() {
    connect_ = false;
    loop_->QueueInLoop(std::bind(&Connector::StopInLoop, this));
}

const InetAddress& Connector::GetPeerAddr() const { return peer_addr_; }

Connector::~Connector() {
    const void* this_address = static_cast<const void*>(this);
    std::stringstream str_this_address;
    str_this_address << this_address;
    LOG_DEBUG("Connector in:" + str_this_address.str() + " earse");
}