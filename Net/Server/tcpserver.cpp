#include "Head/tcpserver.h"
#include "../Poll/Head/eventloop.h"

using namespace Wasi::Server;

TcpServer::TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr,
                     const std::string& name_, OptReusePort opt) :
    loop(loop_),
    name(name_),
    acceptor(new Acceptor(loop, listenaddr, (opt == REUSEPORT))),

{

}

/*
