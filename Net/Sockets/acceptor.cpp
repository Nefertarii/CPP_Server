#include "Head/acceptor.h"
#include "Head/socketapi.h"
#include "../Poll/Head/eventloop.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>

using namespace Wasi::Sockets;

void Acceptor::Handle_read() {
    loop->Assert_in_loop_thread();
    InetAddress peeraddr;
    int connfd = accept_socket.Accept(&peeraddr);
    if (connfd >= 0) {
        if (callback) { callback(connfd, peeraddr); }
        else { Close(connfd); }
    }
    else {
        std::cout << "Acceptor::Handle_read error\n";
        if (errno == EMFILE) {
            close(idle_fd);
            idle_fd = accept(accept_socket.Fd(), nullptr, nullptr);
            close(idle_fd);
            idle_fd = open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}

Acceptor::Acceptor(Poll::EventLoop* loop_, const InetAddress& listen_addr, bool reuse_port) :
    loop(loop_),
    accept_socket(Create_socket(listen_addr.Family())),
    accept_channel(loop, accept_socket.Fd()),
    listening(false),
    idle_fd(open("/dev/null", O_RDONLY | O_CLOEXEC)) {
    assert(idle_fd >= 0);
    accept_socket.Set_reuse_addr(true);
    accept_socket.Set_reuse_port(reuse_port);
    accept_socket.Bind_address(listen_addr);
    accept_channel.Set_read_callback(std::bind(&Acceptor::Handle_read, this));
}

void Acceptor::Set_new_connection_callback(const NewConnectCallback& callback_) {
    callback = callback_;
}

void Acceptor::Listen() {
    loop->Assert_in_loop_thread();
    listening = true;
    accept_socket.Listen();
    accept_channel.Enable_reading();
}

bool Acceptor::Listening() const {
    return listening;
}

Acceptor::~Acceptor() {
    accept_channel.Disable_reading();
    accept_channel.Disable_writing();
    close(idle_fd);
}