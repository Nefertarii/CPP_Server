#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Net/tcpserver.h"
#include "Include/Timer/clock.h"
#include <iostream>

using namespace wasi;
using namespace wasi::net;
using namespace wasi::loop;


void ConnectFunc(const TcpConnectionPtr& conn_ptr) {
    if (conn_ptr->Connected()) {
        std::cout << "Get Connection";
    }
}

void ReadFunc(const TcpConnectionPtr& conn_ptr, std::string* read_buf, timer::TimeStamp time) {
    std::cout << "Get message:" << *read_buf << "\n";
    std::cout << "At " << timer::Clock::ToStringSec(time.GetTime()/1000) << "\n";
    std::string test_str(300, 'a');
    conn_ptr->Send(test_str);
};

void WriteCompleteFunc(const TcpConnectionPtr& conn_ptr) {
    conn_ptr->ForceClose();
    std::cout << "Connection Close\n";
};

void Tcpserver_t() {
    EventLoop main_loop;
    const InetAddress listen("127.0.0.1", 9988);
    TcpServer tcp_server(&main_loop, listen, "test server");
    tcp_server.SetConnectionCallback(ConnectFunc);
    tcp_server.SetMessageCallback(ReadFunc);
    tcp_server.SetWriteCompleteCallback(WriteCompleteFunc);
    tcp_server.Start();
    main_loop.Loop();
};

int main() {
    log::Logger::Init();
    Tcpserver_t();
}