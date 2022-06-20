#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpclient.h>
#include <iostream>

using namespace Wasi;
using namespace std;

void Connection(const Server::TcpConnectionPtr& conn) {
    std::string msg = "GET /Resource/CSS/Footer.css HTTP/1.1";
    conn->Send(msg);
}

void Message(const Server::TcpConnectionPtr& conn, Base::Buffer* read, Time::TimeStamp time) {
    std::cout << conn->Get_input_buffer() << "\nend\n";
}

void func1() {
    Poll::EventLoop main_loop;
    Sockets::InetAddress cli_address("127.0.0.1", 8000);
    Server::TcpClient client(&main_loop, cli_address, "client");
    client.Set_connection_callback(Connection);
    client.Set_message_callback(Message);
    client.Connect();
    main_loop.Loop();
}

int main() {
    func1();
}