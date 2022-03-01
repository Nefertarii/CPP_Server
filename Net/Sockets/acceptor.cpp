#include "Head/acceptor.h"
#include "../Poll/Head/eventloop.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using namespace Wasi::Sockets;

Acceptor::Acceptor(Poll::EventLoop* loop_, const InetAddress& listen_addr, bool reuse_port) :
    loop(loop_),
{

}

void Acceptor::Set_new_connection_callback(const NewConnectCallback& callback_);
void Acceptor::Listen();
bool Acceptor::Listening() const;
Acceptor::~Acceptor();