#include "Include/Net/tcpconnection.h"
#include "Include/Net/socket.h"
#include "Include/Net/socketpack.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Eventloop/channel.h"
#include "Include/Timer/clock.h"
#include "Include/Logger/logger.h"
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/uio.h>
#include <sys/fcntl.h>

using namespace wasi;
using namespace wasi::net;

void TcpConnection::HandleRead(timer::TimeStamp receive_time) {
    if (loop_->IsInLoopThread()) {
        iovec vec;
        char extrabuf[65536];
        vec.iov_base       = extrabuf;
        vec.iov_len        = sizeof(extrabuf);
        const int iovcnt   = 1;
        const ssize_t read = readv(channel_->GetFd(), &vec, iovcnt);
        output_string_     = extrabuf;
        //LOG_DEBUG(output_string_);
        // SocketPack::Read(channel_->GetFd(), );
        if (read > 0) {
            message_callback_(shared_from_this(), &output_string_, receive_time);
        } else if (read == 0) {
            HandleClose();
        } else {
            LOG_ERROR("errno:" + std::to_string(errno));
            HandleError();
        }
    }
}

void TcpConnection::HandleWrite() {
    if (loop_->IsInLoopThread()) {
        if (channel_->Writing()) {
            ssize_t wrote = SocketPack::Write(channel_->GetFd(), output_string_.c_str(),
                                              output_string_.size());
            if (wrote > 0) {
                output_string_ = std::string(output_string_, wrote, output_string_.size());
                if (output_string_.size() == 0) {
                    channel_->DisableWriting();
                    if (write_complete_callback_) { loop_->QueueInLoop(
                        std::bind(write_complete_callback_, shared_from_this())); }
                    if (state_ == kConnectdisconnecting) { Shutdown(); }
                    return;
                }
            } else {
                LOG_ERROR("errno:" + std::to_string(errno));
                HandleError();
            }
        }
    }
}

void TcpConnection::HandleClose() {
    if (loop_->IsInLoopThread()) {
        if (state_ != kConnectconnected || state_ != kConnectdisconnecting) {
            SetState(kConnectdisconnected);
            channel_->DisableAll();
            TcpConnectionPtr guard_this(shared_from_this());
            connection_callback_(guard_this);
            close_callback_(guard_this);
        } else {
            LOG_ERROR("TcpConnection " + name_ + "] " + " state_ :" + StringState(state_));
        }
    }
}

void TcpConnection::HandleError() {
    int err = SocketPack::GetSocketError(channel_->GetFd());
    LOG_WARN("TcpConnection " + name_ + " error code:" + std::to_string(err));
}

void TcpConnection::SendInLoop() {
    if (state_ == kConnectdisconnected) {
        LOG_INFO("connection is disconnected");
        return;
    }
    if (loop_->IsInLoopThread()) {
        ssize_t wrote    = 0;
        size_t length    = input_string_.size();
        size_t remaining = length;
        bool fault       = false;
        if (!channel_->Writing() && remaining != 0) {
            wrote = SocketPack::Write(channel_->GetFd(), input_string_.c_str(), length);
            if (wrote > 0) {
                remaining = length - wrote;
                if (remaining == 0 && write_complete_callback_) {
                    input_string_.clear();
                    loop_->QueueInLoop(std::bind(write_complete_callback_, shared_from_this()));
                } else {
                    input_string_ = std::string(input_string_, wrote, input_string_.size());
                }
            } else {
                if (errno != EWOULDBLOCK) {
                    if (errno == EPIPE || errno == ECONNRESET) {
                        fault = true;
                    }
                    LOG_ERROR("Send " + peer_addr_.GetIpPort() + " error, code:" + std::to_string(errno));
                }
            }
        }
        if (remaining >= length) {
            // high_water_mark
            if (!fault && remaining > 0) {
                if (length + remaining >= high_water_mark_
                    && length < high_water_mark_
                    && high_water_mark_callback_) {
                    loop_->QueueInLoop(std::bind(high_water_mark_callback_, shared_from_this(), length + remaining));
                }
                if (!channel_->Writing()) { channel_->EnableWriting(); }
                // writing not writeing
            }
        }
    }
}

void TcpConnection::SendFileInLoop() {
    if (state_ == kConnectdisconnected) {
        LOG_INFO("connection is disconnected");
        return;
    }
    size_t once_send = 15000;
    if (loop_->IsInLoopThread()) {
        if (!channel_->Writing() && file_.Remaining() > 0) {
            ssize_t ret = sendfile(socket_->GetFd(), file_.file_fd, &file_.file_offset, once_send);
            if (ret > 0) {
                if (file_.Remaining() > 0) {
                    loop_->QueueInLoop(std::bind(&TcpConnection::SendFileInLoop, shared_from_this()));
                    return;
                }
                if (write_complete_callback_) {
                    close(file_.file_fd);
                    file_.Clear();
                    loop_->QueueInLoop(std::bind(write_complete_callback_, shared_from_this()));
                    return;
                }
                LOG_INFO("request file:" + file_.target_path + " send done");
                close(file_.file_fd);
                file_.Clear();
            } else {
                LOG_ERROR("send file to:" + peer_addr_.GetIpPort() + " error, code:" + std::to_string(errno));
                if (errno == EAGAIN) {
                    LOG_DEBUG("resource in using");
                    loop_->RunAfter(0.05, std::bind(&TcpConnection::SendFileInLoop, shared_from_this()));
                } else {
                    ConnectDestroyed();
                }
            }
        }
    }
}

void TcpConnection::ShutdownInLoop() {
    if (loop_->IsInLoopThread()) {
        if (!channel_->Writing()) {
            socket_->CloseWrite();
        }
    }
}

void TcpConnection::ForceCloseInLoop() {
    if (loop_->IsInLoopThread()) {
        if (state_ == kConnectconnected || state_ == kConnectdisconnecting) {
            HandleClose();
        }
    }
}

void TcpConnection::StartReadInLoop() {
    if (loop_->IsInLoopThread()) {
        if (!reading_ || !channel_->Reading()) {
            channel_->EnableReading();
            reading_ = true;
        }
    }
}

void TcpConnection::StopReadInLoop() {
    if (loop_->IsInLoopThread()) {
        if (reading_ || channel_->Reading()) {
            channel_->DisableReading();
            reading_ = false;
        }
    }
}

void TcpConnection::SetState(ConnectState state) {
    state_ = state;
    LOG_DEBUG(peer_addr_.GetIpPort() + " now is " + StringState(state_));
}

std::string TcpConnection::StringState(ConnectState state) {
    switch (state) {
    case kConnectdisconnected:
        return "disconnected";
    case kConnectconnecting:
        return "connecting";
    case kConnectconnected:
        return "connected";
    case kConnectdisconnecting:
        return "disconnecting";
    default:
        return "unknown state";
    }
}

TcpConnection::TcpConnection(loop::EventLoop* loop, const std::string& name, int sockfd,
                             const net::InetAddress& local_addr,
                             const net::InetAddress& peer_addr) :
    loop_(loop),
    name_(name),
    state_(kConnectconnecting),
    reading_(true),
    high_water_mark_(64 * 1024 * 1024),
    input_string_(),
    output_string_(),
    context_(),
    file_(),
    socket_(std::make_unique<Socket>(sockfd)),
    channel_(std::make_unique<loop::Channel>(loop, sockfd)),
    local_addr_(local_addr),
    peer_addr_(peer_addr),
    connection_callback_(nullptr),
    message_callback_(nullptr),
    write_complete_callback_(nullptr),
    high_water_mark_callback_(nullptr),
    close_callback_(nullptr) {
    channel_->SetReadCallback(std::bind(&TcpConnection::HandleRead, this, timer::TimeStamp(timer::Clock::NowtimeMs())));
    channel_->SetWriteCallback(std::bind(&TcpConnection::HandleWrite, this));
    channel_->SetCloseCallback(std::bind(&TcpConnection::HandleClose, this));
    channel_->SetErrorCallback(std::bind(&TcpConnection::HandleError, this));
    socket_->SetKeepAlive(true);
}

loop::EventLoop* TcpConnection::GetLoop() const { return loop_; }

const std::string& TcpConnection::GetName() const { return name_; }

const net::InetAddress& TcpConnection::GetLocalAddress() const { return local_addr_; }

const net::InetAddress& TcpConnection::GetPeerAddress() const { return peer_addr_; }

std::string TcpConnection::GetInputString() { return input_string_; }

std::string TcpConnection::GetOutputString() { return output_string_; }

head::FileStat TcpConnection::GetFileStat() { return file_; }

std::string TcpConnection::GetTcpInfo() const {
    std::string tcp_info;
    socket_->GetTcpInfo(&tcp_info);
    return tcp_info;
}

std::any TcpConnection::GetContext() { return context_; }

bool TcpConnection::GetTcpInfo(tcp_info* tcpi) const { return socket_->GetTcpInfo(tcpi); }

bool TcpConnection::Connected() const { return state_ == kConnectconnected; }

bool TcpConnection::Disconnected() const { return state_ == kConnectdisconnected; }

bool TcpConnection::Reading() const { return reading_; }

void TcpConnection::Send(const std::string message) {
    input_string_ = message;
    if (state_ == kConnectconnected) {
        if (loop_->IsInLoopThread()) {
            SendInLoop();
        } else {
            loop_->RunInLoop(std::bind(&TcpConnection::SendInLoop, shared_from_this()));
        }
    }
}

void TcpConnection::Send(const std::string message, size_t len) {
    std::string clip_str = message.substr(0, len);
    Send(clip_str);
}

void TcpConnection::Send(const char* message, size_t len) {
    std::string string(message, 0, len);
    Send(string);
}

void TcpConnection::SendFile(const std::string filename) {
    if (filename.empty()) { return; }
    struct stat sys_file_stat;
    if (file_.target_path.empty()) {
        file_.target_path = filename.c_str();
    }
    if (stat(file_.target_path.c_str(), &sys_file_stat) < 0) {
        LOG_ERROR("not this file:" + filename);
    } else {
        if (file_.file_length == 0) {
            file_.file_length = sys_file_stat.st_size;
        }
        file_.file_fd = open(filename.c_str(), O_RDONLY);
        if (state_ == kConnectconnected) {
            if (loop_->IsInLoopThread()) {
                SendFileInLoop();
            } else {
                loop_->RunInLoop(std::bind(&TcpConnection::SendFileInLoop, shared_from_this()));
            }
        }
    }
}

void TcpConnection::Shutdown() {
    if (state_ == kConnectconnected) {
        SetState(kConnectdisconnecting);
        loop_->RunInLoop(std::bind(&TcpConnection::ShutdownInLoop, shared_from_this()));
    }
}

void TcpConnection::ForceClose() {
    if (state_ == kConnectconnected || state_ == kConnectdisconnecting) {
        SetState(kConnectdisconnecting);
        loop_->RunInLoop(std::bind(&TcpConnection::ForceCloseInLoop, shared_from_this()));
    }
}

void TcpConnection::ForceCloseDelay(double seconds) {
    if (state_ == kConnectconnected || state_ == kConnectdisconnecting) {
        SetState(kConnectdisconnecting);
        loop_->RunAfter(seconds, std::bind(&TcpConnection::ForceClose, shared_from_this()));
    }
}

void TcpConnection::StartRead() {
    loop_->RunInLoop(std::bind(&TcpConnection::StartReadInLoop, shared_from_this()));
}

void TcpConnection::StopRead() {
    loop_->RunInLoop(std::bind(&TcpConnection::StopReadInLoop, shared_from_this()));
}

void TcpConnection::SetNoDelay(bool flag) { socket_->SetTcpDelay(flag); }

void TcpConnection::SetContext(const std::any& context) { context_ = context; }

void TcpConnection::SetConnectionCallback(const ConnectionCallback& func) {
    connection_callback_ = func;
}

void TcpConnection::SetOutputString(std::string message) { output_string_ = message; }

void TcpConnection::SetMessageCallback(const MessageCallback& func) {
    message_callback_ = func;
}

void TcpConnection::SetWriteCompleteCallback(const WriteCompleteCallback& func) {
    write_complete_callback_ = func;
}

void TcpConnection::SetHighWaterMarkCallback(const HighWaterMarkCallback& func, size_t high_water_mark) {
    high_water_mark_callback_ = func;
    high_water_mark_          = high_water_mark;
}

void TcpConnection::SetCloseCallback(const CloseCallback& func) {
    close_callback_ = func;
}

void TcpConnection::ConnectEstablished() {
    if (loop_->IsInLoopThread()) {
        if (state_ == kConnectconnecting) {
            SetState(kConnectconnected);
            channel_->EnableReading();
            connection_callback_(shared_from_this());
        }
    }
}

void TcpConnection::ConnectDestroyed() {
    if (loop_->IsInLoopThread()) {
        if (state_ == kConnectconnected) {
            SetState(kConnectdisconnected);
            channel_->DisableAll();
            connection_callback_(shared_from_this());
        }
        channel_->Remove();
    }
}

TcpConnection::~TcpConnection() {
    if (state_ != kConnectdisconnected) {
        LOG_WARN("connect still alive");
    }
}