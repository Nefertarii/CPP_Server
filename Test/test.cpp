#include <iostream>
#include <thread>
#include "../Headfile/Thread/vector.hpp"
#include "../Headfile/Thread/list.hpp"
#include "../Headfile/Thread/queue.hpp"
#include "../Headfile/Thread/deque.hpp"
#include "../Headfile/Thread/stack.hpp"
#include "../Headfile/Thread/map.hpp"
#include "../Headfile/Thread/threadpool.hpp"
#include "../Headfile/Timer/Head/timer.h"
#include "../Log/Head/formatter.h"
#include "../Log/Head/sink.h"
#include "../Log/Head/core.h"
#include "../Headfile/Net/Head/netlog.h"
#include "../Headfile/Net/Poll/Head/eventloop.h"
#include "../Headfile/Net/Poll/Head/channel.h"
#include "../Headfile/Net/Poll/Head/poll.h"
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

Wasi::Thread::Safe_Queue<int> ique;
Wasi::Thread::Safe_Deque<int> ideq;
Wasi::Thread::Safe_Vector<int> ivec;
Wasi::Thread::Safe_Stack<int> istk;
Wasi::Thread::Safe_List<int> ilist;
Wasi::Thread::Safe_Map<int, int> iimap;
Wasi::Thread::Thread_Pool threads;

void test_func1() {
    std::cout << "test_func 1\n";
}

int test_func2() {
    std::cout << "test_func 2\nreturn 42\n";
    return 42;
}

void test_func3() {
    uint i = 0;
    while (i < 20) {
        std::cout << "test_func 3\n";
        i++;
    }
}

/*Event_Loop eventloop;

void func2() {
    std::cout << "func2 run.\n";
    eventloop.Loop();
}

void func1() {
    std::cout << "func1 run.\n";
    eventloop.Loop();
}
//std::cout << "|" << sink.Log_consume() << "|";
*/

void func1() {
    TimeStamp t1;
    TimeStamp t2(Timer::Nowtime_us());
    std::cout << t2.Microseconds_since_epoch() << "\n";
    std::cout << Timer::Nowtime_us() << "\n";
    std::cout << Timer::Nowtime_ms() << "\n";
    std::cout << Timer::Nowtime_sec() << "\n";
    std::cout << Timer::To_string(t2);
}

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
    channel.Set_read_callback(timeout);
    channel.Enable_reading();
    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);
    loop2.Loop();
    ::close(timerfd);
}

int main() {
    //thread T1(func1);
    //thread T2(func2);
    func2();
}