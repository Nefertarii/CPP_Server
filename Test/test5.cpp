#include <Base/Poll/eventloop.h>
#include <Net/Tcp/tcpclient.h>
#include <Net/Tcp/tcpserver.h>
#include <Base/Timer/clock.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <arpa/inet.h>

using namespace std;
using namespace Wasi::Base;
using namespace Wasi::Server;
using namespace Wasi::Poll;
using namespace Wasi::Sockets;
using namespace Wasi::Time;

void Connection(const TcpConnectionPtr& conn) {
    std::string message = "GET / HTTP/1.1\r\n";
    message += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n";
    message += "Accept-Encoding: gzip, deflate, br\r\n";
    message += "Accept-Language: en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7\r\n";
    message += "Cache-Control: no-cache\r\n";
    message += "Connection: keep-alive\r\n";
    message += "Host: 127.0.0.1:8000\r\n";
    message += "Pragma: no-cache\r\n";
    message += "sec-ch-ua-mobile: ?0\r\n";
    message += "Sec-Fetch-Dest: document\r\n";
    message += "Sec-Fetch-Mode: navigate\r\n";

    // while (1) {
    if (conn->Connected()) {
        std::cout << "Connection: new connection [" << conn->Get_name() << "]"
                  << " from " << conn->Get_peer_address().To_string_ip() << "\n";
        conn->Send(message);
    } else {
        std::cout << "Connection: connection [" << conn->Get_name() << "]"
                  << " is down\n";
    }

    //}
}

void Message(const TcpConnectionPtr& conn, Buffer* buffer,
             TimeStamp receive_time) {
    std::cout << "Message: received " << buffer->Size()
              << " bytes from connection [" << conn->Get_name() << "] at "
              << Clock::To_string(receive_time) << "\n["
              << buffer->Content() << "]\n";
    buffer->Init();
}

void process(TcpClient* client) {
    TcpConnectionPtr conn = client->Connection();
    if (conn) {
        this_thread::sleep_for(chrono::seconds(1));
        conn->Send("1");
    }
}

void func1() {
    signal(SIGPIPE, SIG_IGN);
    EventLoop loop;
    InetAddress servaddr("127.0.0.1", 9999);
    TcpClient client(&loop, servaddr, "client1");
    client.Set_connection_callback(Connection);
    client.Set_message_callback(Message);
    client.Enable_retry();
    client.Connect();
    // loop.Run_every(2.0, std::bind(process, &client));
    loop.Loop();

    // cout << "\n\n\n\n";
}

void func2() {
    int sockfd = 0;
    sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    sockfd              = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(9981);
    std::string str3    = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890~!@#$%^&*(){}:\">?\n";
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    std::cout << "connect:" << connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) << "\n";
    sleep(1);
    std::cout << "write:" << write(sockfd, str3.c_str(), str3.size()) << "bytes\n";
    close(sockfd);
}

int main() {
    func1();
}