#include <iostream>
#include <thread>
#include "../Headfile/Thread/vector.hpp"
#include "../Headfile/Thread/list.hpp"
#include "../Headfile/Thread/queue.hpp"
#include "../Headfile/Thread/deque.hpp"
#include "../Headfile/Thread/stack.hpp"
#include "../Headfile/Thread/map.hpp"
#include "../Headfile/Thread/threadpool.hpp"
#include "../Headfile/Timer/Head/clock.h"
#include "../Headfile/Timer/Head/timerid.h"
#include "../Log/Head/formatter.h"
#include "../Log/Head/sink.h"
#include "../Log/Head/core.h"
#include "../Headfile/Net/Head/netlog.h"
#include "../Headfile/Net/Poll/Head/eventloop.h"
#include "../Headfile/Net/Poll/Head/channel.h"
#include "../Headfile/Net/Poll/Head/poller.h"
#include "../Headfile/Net/Poll/Head/eventloopthread.h"

#include <sys/timerfd.h>

/* using/enum  AaBbCc
 * class/struct Aa_Bb_Cc
 * func Aa_bb_cc
 * var aa_bb_cc
 *
 */

 //using namespace Wasi::Log; 
using namespace Wasi::Net;
using namespace Wasi::Time;
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

int main() {
    //thread T1(func1);
    //thread T2(func2);
    //func2();
    func7();
    //EventLoop loop;
}