#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpserver.h>
#include <Base/Timer/clock.h>
#include <iostream>

using namespace Wasi;

void ConnectFunc(const Server::TcpConnectionPtr& conn_ptr) {
    if (conn_ptr->Connected()) {
        std::cout << "Get Connection";

    }
}

void ReadFunc(const Server::TcpConnectionPtr& conn_ptr, Base::Buffer* read_buf, Time::TimeStamp time) {
    std::cout << "Get message:" << read_buf->Content() << "\n";
    std::cout << "At " << Time::Clock::To_string_sec(time.Microseconds_since_epoch()) << "\n";
    std::string test_str(3000, 'a');
    conn_ptr->Send(test_str);
};

void WriteCompleteFunc(const Server::TcpConnectionPtr& conn_ptr) {
    conn_ptr->Force_close();
    std::cout << "Connection Close\n";
};

void StartFunc() {
    Poll::EventLoop main_loop;
    const Sockets::InetAddress listen("127.0.0.1", 9999);
    Server::TcpServer tcp_server(&main_loop, listen, "test server");
    tcp_server.Set_connection_callback(ConnectFunc);
    tcp_server.Set_message_callback(ReadFunc);
    tcp_server.Set_write_complete_callback(WriteCompleteFunc);
    tcp_server.Start();
    main_loop.Loop();
};

int main() {
    StartFunc();
}