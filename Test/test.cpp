#include "../Net/Base/Head/buffer.h"
#include "../Net/Poll/Head/channel.h"
#include "../Net/Poll/Head/eventloop.h"
#include "../Net/Poll/Head/eventloopthread.h"
#include "../Net/Poll/Head/poller.h"
#include "../Net/Server/Head/tcpserver.h"
#include "../Net/Sockets/Head/acceptor.h"
#include "../Net/Sockets/Head/inetaddress.h"
#include "../Net/Sockets/Head/socket.h"
#include "../Net/Sockets/Head/socketapi.h"
#include "../Thread/deque.hpp"
#include "../Thread/list.hpp"
#include "../Thread/map.hpp"
#include "../Thread/queue.hpp"
#include "../Thread/stack.hpp"
#include "../Thread/threadpool.hpp"
#include "../Thread/vector.hpp"
#include "../Timer/Head/clock.h"
#include "../Timer/Head/timerid.h"
#include <iostream>
#include <thread>

#include <sys/timerfd.h>

/* using/enum  AaBbCc
 * class/struct Aa_Bb_Cc
 * func Aa_bb_cc
 * var aa_bb_cc
 *
 */

using namespace Wasi::Poll;
using namespace Wasi::Base;
using namespace Wasi::Time;
using namespace Wasi::Sockets;
using namespace Wasi::Server;
using namespace std;

void New_connection(int sockfd, const InetAddress& peeraddr) {
    cout << "New_connection(): accepted a new connection from"
         << peeraddr.To_string_ip_port() << "\n";
    Write(sockfd, "How are you?\n", 13);
}

void func8() {
    cout << "main pid:" << getpid() << "\n";
    InetAddress listenaddr(9981);
    EventLoop loop;
    Acceptor acceptor(&loop, listenaddr, true);
    acceptor.Set_new_connection_callback(New_connection);
    acceptor.Listen();

    loop.Loop();
}

void New_connection2(const TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        std::cout << "New_connection(): new connection [" << conn->Get_name()
                  << "] form " << conn->Get_peer_address().To_string_ip_port() << "\n";
    } else {
        std::cout << "New_connection(): connection [" << conn->Get_name()
                  << "] is down\n";
    }
}

void Message(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime) {
    if (buf->State()) {
        std::string message = "How are you?";
        conn->Send(message);
        std::cout << "Send_message: Send " << message.size()
                  << " bytes from connection [" << conn->Get_name() << "] at "
                  << Clock::To_string(receiveTime) << "\n";
    } else {
        std::cout << "Read_message: received:"
                  << buf->Content() << "\nTotal:" << buf->Size()
                  << " bytes from connection [" << conn->Get_name() << "] at "
                  << Clock::To_string(receiveTime) << "\n";
    }
}

void Send_message(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime) {
}

void func9() {
    std::cout << "main pid:" << getpid() << "\n";
    InetAddress listenaddr(9981);
    EventLoop loop;
    TcpServer server(&loop, listenaddr, "server1", TcpServer::REUSEPORT);
    server.Set_connection_callback(New_connection2);
    server.Set_message_callback(Message);
    server.Start();
    loop.Loop();
}

int main() {
    func9();
}