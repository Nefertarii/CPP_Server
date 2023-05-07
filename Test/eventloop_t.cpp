#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Timer/clock.h"
#include <sys/timerfd.h>
#include <iostream>

using namespace wasi;
using namespace wasi::loop;
using namespace std;

void print1() {
    cout << "timer RunAt expired!\n";
}
void print2() {
    cout << "timer After expired!\n";
}
void print3() {
    cout << "timer Every expired!\n";
}

void TimerQueue_t() {
    /*
     * TimerQueue timer_queue;
     * cout << "create fd:" << timer_queue.timerfd_ << "\n";
     * timer::TimeStamp timestamp1(timer::Clock::NowtimeMs() + 40 * 1000);
     * timespec ret = timer_queue.TimeFromNow(timestamp1);
     * cout << "time from now:" << ret.tv_sec << " sec \n";
     *
     * timer::TimeStamp timestamp2(timer::Clock::NowtimeMs() + 2 * 1000);
     * timer::Timer* timer1 = new timer::Timer(print, timestamp2, 0.0);
     * timer_queue.AddTimer(timer1);
     * cout << "wait...\n";
     * std::vector<Expired> epireds = timer_queue.GetExpired(timer::Clock::NowtimeMs());
     * cout << "get:" << epireds.size() << " expired timer\n";
     * std::this_thread::sleep_for(timer::Timer_Sec(3));
     * epireds = timer_queue.GetExpired(timer::Clock::NowtimeMs());
     * cout << "get:" << epireds.size() << " expired timer\n";
     * cout << "done\n";
     */
    // all function public test
}

void EventLoop_t() {
    EventLoop loop;
    loop.RunAt(timer::Clock::NowtimeMs() + 10 * 1000, print1);
    loop.RunAfter(5.0, print2);
    loop.RunEvery(2.0, print3);
    loop.Loop();
}

int main() {
    log::Logger::Init();
    EventLoop_t();

}