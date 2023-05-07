#include "Include/Net/acceptor.h"
#include "Include/Net/socketpack.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include <fcntl.h>

using namespace wasi;
using namespace wasi::net;

void Acceptor::HandleRead() {
    if (eventloop_->IsInLoopThread()) {
        InetAddress peeraddr;
        int connfd = socket_.Accept(&peeraddr);
        if (connfd >= 0) {
            if (connected_func) {
                connected_func(connfd, peeraddr);
            } else {
                SocketPack::Close(connfd);
            }
        } else {
            int tmp_errno = errno;
            LOG_ERROR("errno:" + std::to_string(tmp_errno));
            if (tmp_errno == EMFILE) {
                close(idle_fd_);
                idle_fd_ = accept(socket_.GetFd(), nullptr, nullptr);
                close(idle_fd_);
                idle_fd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
            }
        }
    }
}

Acceptor::Acceptor(loop::EventLoop* eventloop, const InetAddress& listen_addr, bool reuse_port) :
    socket_(SocketPack::CreateSocket(listen_addr.GetFamily())),
    eventloop_(eventloop),
    channel_(eventloop_, socket_.GetFd()),
    connected_func(nullptr),
    listening_(false),
    idle_fd_(open("/dev/null", O_RDONLY | O_CLOEXEC)) {
    if (idle_fd_ < 0) {
        LOG_WARN("idle_fd_ not created correctly");
    }
    if (reuse_port) {
        if (socket_.SetReuseAddr(reuse_port) != 0) {
            LOG_WARN("reuse addr set fail, errno:" + std::to_string(errno));
        }
        if (socket_.SetReusePort(reuse_port) != 0) {
            LOG_WARN("reuse port set fail, errno:" + std::to_string(errno));
        }
    }
    socket_.Bind(listen_addr);
    channel_.SetReadCallback(std::bind(&Acceptor::HandleRead, this));
}

void Acceptor::SetNewConnectionCallback(const NewConnectCallback& func) {
    connected_func = func;
}

void Acceptor::Listen() {
    eventloop_->IsInLoopThread();
    listening_ = true;
    socket_.Listen();
    channel_.EnableReading();
}

bool Acceptor::Listening() const { return listening_; }

Acceptor::~Acceptor() {
    channel_.DisableAll();
    close(idle_fd_);
}