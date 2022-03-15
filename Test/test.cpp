#include <iostream>
#include <thread>
#include "../Thread/vector.hpp"
#include "../Thread/list.hpp"
#include "../Thread/queue.hpp"
#include "../Thread/deque.hpp"
#include "../Thread/stack.hpp"
#include "../Thread/map.hpp"
#include "../Thread/threadpool.hpp"
#include "../Timer/Head/clock.h"
#include "../Timer/Head/timerid.h"
#include "../Log/Head/formatter.h"
#include "../Log/Head/sink.h"
#include "../Log/Head/core.h"
#include "../Net/Base/Head/netlog.h"
#include "../Net/Poll/Head/eventloop.h"
#include "../Net/Poll/Head/channel.h"
#include "../Net/Poll/Head/poller.h"
#include "../Net/Poll/Head/eventloopthread.h"
#include "../Net/Sockets/Head/acceptor.h"
#include "../Net/Sockets/Head/inetaddress.h"
#include "../Net/Sockets/Head/socketapi.h"
#include "../Net/Sockets/Head/socket.h"
#include "../Net/Base/Head/buffer.h"
#include "../Net/Server/Head/tcpserver.h"

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

void NewConnection(int sockfd, const InetAddress& peeraddr) {
    cout << "NewConnection(): accepted a new connection from"
        << peeraddr.To_string_ip_port() << "\n";
    Write(sockfd, "How are you?\n", 13);
}

void func8() {
    cout << "main pid:" << getpid() << "\n";
    InetAddress listenaddr(9981);
    EventLoop loop;
    Acceptor acceptor(&loop, listenaddr, true);
    acceptor.Set_new_connection_callback(NewConnection);
    acceptor.Listen();

    loop.Loop();
}

void NewConnection2(const TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        std::cout << "onConnection(): new connection [" << conn->Get_name()
            << "] form" << conn->Get_peer_address().To_string_ip_port() << "\n";
    }
    else {
        std::cout << "onConnection(): connection [" << conn->Get_name()
            << "] is down\n";
    }
}

void Message(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime) {
    std::cout << "Message(): received" << buf->Size()
        << "bytes from connection [" << conn->Get_name() << "] at "
        << Clock::To_string(receiveTime);
}

void func9() {
    std::cout << "main pid:" << getpid() << "\n";
    InetAddress listenaddr(9981);
    EventLoop loop;
    TcpServer server(&loop, listenaddr, "server1", TcpServer::REUSEPORT);
    server.Set_connection_callback(NewConnection2);
    server.Set_message_callback(Message);
    server.Start();
    loop.Loop();
}

int main() {
    func9();
}