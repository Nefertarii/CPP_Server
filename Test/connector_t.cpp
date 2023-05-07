#include "Include/Net/connector.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Net/socketpack.h"
#include "Include/Logger/logger.h"
#include <iostream>

using namespace wasi;
using namespace wasi::net;
using namespace std;

void OnConnect(int sockfd) {

}

void print() {
    cout << "print in:" << gettid() << "\n";
}

void Connector_t() {
    loop::EventLoop loop;
    InetAddress peer_addr("127.0.0.1", 9999);
    Connector connector(&loop, peer_addr);
    connector.SetNewConnectionCallback(OnConnect);
    connector.Start();
    loop.Loop();
}

int main() {
    log::Logger::Init();
    Connector_t();
}