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


EventLoop* loop1;

void timeout() {
    std::cout << "Timeout\n";
    loop1->Quit();
}

void func2() {
    EventLoop loop2;
    loop1 = &loop2;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop2, timerfd);
    channel.Set_read_callback(std::bind(timeout));
    channel.Enable_reading();
    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);
    loop2.Loop();
    ::close(timerfd);
}


int cnt = 0;
EventLoop* loop3;

void printTid() {
    std::cout << "pid = " << getpid()
        << " tid = " << gettid()
        << " now = " << Clock::Nowtime_us() << "\n";
}

void print(const char* msg) {
    std::cout << msg << ":" << Clock::Nowtime_us() << "\n";
    if (++cnt == 20) {
        loop3->Quit();
    }
}

void func4() {
    printTid();
    EventLoop loop;
    loop3 = &loop;
    print("main");
    loop.Run_after(1, std::bind(print, "once1"));
    loop.Run_after(1.5, std::bind(print, "once1.5"));
    loop.Run_after(2.5, std::bind(print, "once2.5"));
    loop.Run_after(3.5, std::bind(print, "once3.5"));
    loop.Run_every(2, std::bind(print, "every2"));
    loop.Run_every(3, std::bind(print, "every3"));

    loop.Loop();
    print("main loop exits");
    sleep(1);
}


EventLoop* g_loop;
int g_flag = 0;

void run4() {
    std::cout << "run4(): pid = " << getpid() << ", flag = " << g_flag << "\n";
    g_loop->Quit();
}

void run3() {
    std::cout << "run3(): pid = " << gettid() << ", flag = " << g_flag << "\n";
    g_loop->Run_after(3, run4);
    g_flag = 3;
}

void run2() {
    std::cout << "run2(): pid = " << getpid() << ", flag = " << g_flag << "\n";
    g_loop->Queue_in_loop(run3);
}

void run1() {
    g_flag = 1;
    std::cout << "run1(): pid = " << getpid() << ", flag = " << g_flag << "\n";
    g_loop->Run_in_loop(run2);
    g_flag = 2;
}

void func5() {
    std::cout << "main(): pid = " << getpid() << ", flag = " << g_flag << "\n";
    EventLoop loop;
    g_loop = &loop;
    loop.Run_after(2, run1);
    loop.Loop();
    std::cout << "main(): pid = " << getpid() << ", flag = " << g_flag << "\n";
}

void func6() {
    std::cout << "func6() pid = " << getpid()
        << ", tid = " << gettid() << "\n";
}

void func7() {
    std::cout << "func7() pid = " << getpid()
        << ", tid = " << gettid() << "\n";
    EventLoopThread loop_thread;
    EventLoop* loop = loop_thread.Start_loop();
    loop->Run_in_loop(func6);
    sleep(1);
    loop->Run_after(2, func6);
    sleep(3);
    loop->Quit();
    printf("exit main().\n");
}

void New_connection(int sockfd, const InetAddress& peeraddr) {
    std::cout << "Accept new connection from" << peeraddr.To_string_ip_port() << "\n";
    //std::cout << write(sockfd, "Hello!\n", 7) << "\n";
    Close(sockfd);
}

void func8() {
    std::cout << "main pid:" << getpid() << "\n";
    InetAddress listean_addr(9002);
    EventLoop loop;
    Acceptor acceptor(&loop, listean_addr, true);
    acceptor.Set_new_connection_callback(New_connection);
    acceptor.Listen();
    loop.Loop();
}

void Connection(const TcpConnectionPtr& conn) {
    if (conn->connected()) {
        std::cout << "onConnection(): new connection [" << conn->Get_name()
            << "] form" << conn->Get_peer_address().To_string_ip_port() << "\n";
    }
    else {
        std::cout << "onConnection(): connection [" << conn->Get_name()
            << "] is down\n";
    }
}

void Message(const TcpConnectionPtr& conn, const char* data, ssize_t len) {
    printf("onMessage(): received %zd bytes from connection [%s]\n",
           len, conn->name().c_str());
    std::cout << "onMessage(): received" << len
        << "bytes from connection [" << conn->Get_name() << "]\n";

}

void func9() {
    std::cout << "main pid:" << getpid() << "\n";
    InetAddress listenaddr(9909);
    EventLoop loop;
    TcpServer server(&loop, listenaddr);
    server.Set_connection_callback(Connection);
    server.Set_message_callback(Message);
    server.Start();
    loop.Loop();
}

int main() {
    //thread T1(func1);
    //thread T2(func2);
    //func2();
    //func4();
    //func5();
    //func6();
    //func7();
    //func8();
    func9();
    //EventLoop loop;
}