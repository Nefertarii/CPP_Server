#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Net/tcpclient.h"
#include "Include/Timer/clock.h"
#include "Include/Net/socketpack.h"
#include <iostream>
#include <cstring>

using namespace wasi;
using namespace wasi::loop;
using namespace wasi::net;

void ConnectFunc(const TcpConnectionPtr& conn_ptr) {
    if (conn_ptr->Connected()) {
        std::string test_str(300, 'a');
        conn_ptr->Send(test_str);
    }
}

void ReadFunc(const TcpConnectionPtr& conn_ptr, std::string* read_buf, timer::TimeStamp time) {
    std::cout << "Get message:" << *read_buf << "\n";
    std::cout << "At " << timer::Clock::ToStringSec(time.GetTime()) << "\n";
};

void WriteCompleteFunc(const TcpConnectionPtr& conn_ptr) {
    conn_ptr->ForceClose();
};

void Tcpclient_t() {
    EventLoop main_loop;
    const InetAddress conn("127.0.0.1", 9999);
    TcpClient Client(&main_loop, conn, "test client");
    Client.SetConnectionCallback(ConnectFunc);
    Client.SetMessageCallback(ReadFunc);
    Client.SetWriteCompleteCallback(WriteCompleteFunc);
    Client.Connect();
    main_loop.RunAfter(10.0, std::bind(&EventLoop::Quit, &main_loop));
    main_loop.Loop();
}

void func2() {
    int sockfd = SocketPack::CreateSocket(AF_INET);
    const InetAddress conn("127.0.0.1", 9999);
    std::string str3    = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890~!@#$%^&*(){}:\">?\n";
    std::cout << "connect:" << conn.GetIpPort() << "\n";
    SocketPack::Connect(sockfd, conn.GetSockaddr());
    sleep(1);
    std::cout << "write:" << write(sockfd, str3.c_str(), str3.size()) << "bytes\n";
    close(sockfd);
}

int main() {
    log::Logger::Init();
    // func2();
    Tcpclient_t();
}