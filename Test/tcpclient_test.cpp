#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpclient.h>
#include <Base/Timer/clock.h>
#include <iostream>

using namespace Wasi;

void ConnectFunc(const Server::TcpConnectionPtr& conn_ptr) {
    if (conn_ptr->Connected()) {
        std::cout << "Get Connection";
        std::string test_str(3000, 'a');
        conn_ptr->Send(test_str);
    }
}

void ReadFunc(const Server::TcpConnectionPtr& conn_ptr, Base::Buffer* read_buf, Time::TimeStamp time) {
    std::cout << "Get message:" << read_buf->Content() << "\n";
    std::cout << "At " << Time::Clock::To_string_sec(time.Microseconds_since_epoch()) << "\n";
};

void WriteCompleteFunc(const Server::TcpConnectionPtr& conn_ptr) {
    conn_ptr->Force_close();
    std::cout << "Connection Close\n";
};

void StartFunc() {
    Poll::EventLoop main_loop;
    const Sockets::InetAddress conn("127.0.0.1", 9999);
    Server::TcpClient Client(&main_loop, conn, "test client");
    Client.Set_connection_callback(ConnectFunc);
    Client.Set_message_callback(ReadFunc);
    Client.Set_write_complete_callback(WriteCompleteFunc);
    Client.Connect();
    main_loop.Loop();
}

int main() {
    StartFunc();
}