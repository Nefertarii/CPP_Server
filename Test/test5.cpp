#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "../Timer/Head/clock.h"
#include "../Net/Base/Head/buffer.h"
#include "../Net/Sockets/Head/socketapi.h"
#include "../Net/Sockets/Head/inetaddress.h"
#include "../Net/Sockets/Head/connector.h"
#include "../Net/Sockets/Head/socketapi.h"
#include "../Net/Poll/Head/eventloop.h"
#include "../Net/Poll/Head/channel.h"
#include "../Net/Server/Head/tcpclient.h"
#include "../Net/Server/Head/tcpconnection.h"

using namespace std;
using namespace Wasi::Base;
using namespace Wasi::Server;
using namespace Wasi::Poll;
using namespace Wasi::Sockets;
using namespace Wasi::Time;

std::string message = "Hello\n";

void Connection(const TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        std::cout << "Connection: new connection [" << conn->Get_name() << "]"
            << " from " << conn->Get_peer_address().To_string_ip() << "\n";
        conn->Send(message);
    }
    else {
        std::cout << "Connection: connection [" << conn->Get_name() << "]"
            << " is down\n";
    }
}

void Message(const TcpConnectionPtr& conn, Buffer* buffer,
             TimeStamp receive_time) {
    std::cout << "Message: received " << buffer->Size()
        << " bytes from connection [" << conn->Get_name() << "] at "
        << Clock::To_string(receive_time) << "\n";
}

void func1() {

    //signal(SIGPIPE, SIG_IGN);
    EventLoop loop;
    InetAddress servaddr("127.0.0.1", 9002);
    TcpClient client(&loop, servaddr, "client1");
    client.Set_connection_callback(Connection);
    client.Set_message_callback(Message);
    client.Enable_retry();
    client.Connect();
    loop.Loop();
}


void func2() {
    int sockfd = 0;
    sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9002);
    std::string str3 = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890~!@#$%^&*(){}:\">?\n";
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    std::cout << "connect:" << connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) << "\n";
    sleep(1);
    std::cout << "write:" << write(sockfd, str3.c_str(), str3.size()) << "bytes\n";
    close(sockfd);
}

int main() {
    func1();
}