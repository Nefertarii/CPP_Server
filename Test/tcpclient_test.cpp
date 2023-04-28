#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpclient.h>
#include <Base/Timer/clock.h>
#include <iostream>

using namespace Wasi;

void ConnectFunc(const Server::TcpConnectionPtr& conn_ptr) {
    if (conn_ptr->Connected()) {
        std::cout << "Get Connection";
         std::string request_string = "GET / HTTP/1.1\r\n"
                                 "Host: www.example.com\r\n"
                                 "User-Agent: Mozilla/5.0\r\n"
                                 "Accept: text/html,application/xhtml+xml\r\n"
                                 "Accept-Language: en-US,en;q=0.5\r\n"
                                 "Accept-Encoding: gzip, deflate\r\n"
                                 "Connection: keep-alive\r\n"
                                 "Referer: http://www.example.com/\r\n"
                                 "Cookie: name=value; name2=value2\r\n"
                                 "\r\n"
                                 "i'm request body, hi";;
        conn_ptr->Send(request_string);
    }
}

void ReadFunc(const Server::TcpConnectionPtr& conn_ptr, Base::Buffer* read_buf, Time::TimeStamp time) {
    std::cout << "Get message:" << read_buf->Content() << "\n";
    std::cout << "At " << Time::Clock::To_string_sec(time.Microseconds_since_epoch()) << "\n";
    conn_ptr->Force_close();
};

void WriteCompleteFunc(const Server::TcpConnectionPtr& conn_ptr) {
    // conn_ptr->Force_close();
    // std::cout << "Connection Close\n";
};

void StartFunc() {
    Poll::EventLoop main_loop;
    const Sockets::InetAddress conn("127.0.0.1", 8000);
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