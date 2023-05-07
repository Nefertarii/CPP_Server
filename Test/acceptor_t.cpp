#include "Include/Net/acceptor.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include <iostream>

using namespace wasi;
using namespace wasi::net;
using namespace std;

void NewConnection(int sockfd, const InetAddress& peer_addr) {
    cout << "accept a new connection from:" << peer_addr.GetIpPort() << "\n";
    SocketPack::Write(sockfd, "Hi!\n", 4);
    SocketPack::Close(sockfd);
}

void Acceptor_t() {
    InetAddress listen_addr("127.0.0.1", 9999);
    loop::EventLoop loop;
    Acceptor acceptor(&loop, listen_addr);
    acceptor.SetNewConnectionCallback(NewConnection);
    acceptor.Listen();
    loop.Loop();
}

int main() {
    log::Logger::Init();
    Acceptor_t();
}