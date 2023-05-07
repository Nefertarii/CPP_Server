#include "Include/Timer/timer.h"
#include "Include/Timer/clock.h"
#include <iostream>

using namespace wasi;
using namespace wasi::timer;
using namespace std;

void TimeStamp_t() {
    TimeStamp stamp1(100000), stamp2(200000), stamp3(500000);
    std::cout << "stamp1 time_:" << stamp1.GetTime() << "\n";
    stamp1.TimeAdd(100.000);
    std::cout << "stamp1 time_:" << stamp1.GetTime() << "\n";
    std::cout << "two stamp diff(1,2):" << TimeStamp::TimeStampDiff(stamp1, stamp2) << "\n";
    stamp2.TimeAdd(300.000);
    std::cout << "stamp2 time_:" << stamp2.GetTime() << "\n";
    std::cout << "two stamp diff(1,2):" << stamp2.TimeDiff(stamp1) << "\n";
    if (stamp1 < stamp2) {
        std::cout << "stamp2 > stamp1\n";
    }
    if (stamp2 == stamp3) {
        std::cout << "stamp2 == stamp3\n";
    }
    stamp1.Swap(stamp2);
    std::cout << "stamp2 time_:" << stamp2.GetTime() << "\n";
    if (stamp1.Valid()) {
        std::cout << "stamp1 time_:" << stamp1.GetTime() << "\n";
    }
    stamp3 = TimeStamp::Invalid();
    std::cout << "stamp3 time_:" << stamp3.GetTime() << "\n";
    ;
}

void Clock_t() {
    Clock clock1;
    long l1 = Clock::NowtimeUs();
    long l2 = Clock::NowtimeMs();
    long l3 = Clock::NowtimeSec();
    cout << l1 << "\n"
         << l2 << "\n"
         << l3 << "\n";
    cout << Clock::ToString(TimeStamp(Clock::NowtimeMs())) << "\n";
    cout << clock1.RuntimeUs() << "\n";
    cout << clock1.RuntimeMs() << "\n";
    cout << clock1.RuntimeSec() << "\n";
}

void PrintNum(int n) { std::cout << "I print:" << n << "\n"; }

void Timer_t() {
    TimeStamp stamp1, stamp2, stamp3;
    stamp1.TimeAdd(1);
    stamp2.TimeAdd(2);
    stamp3.TimeAdd(3);
    cout << "CreateNum" << Timer::CreateNum() << "\n";
    std::function<void()> callback1 = std::bind(PrintNum, stamp1.GetTime());
    std::function<void()> callback2 = std::bind(PrintNum, stamp2.GetTime());
    std::function<void()> callback3 = std::bind(PrintNum, stamp3.GetTime());
    Timer timer1(callback1, stamp1, 0);
    cout << "CreateNum" << timer1.Sequence() << "\n";
    Timer timer2(callback2, stamp2, 0);
    cout << "CreateNum" << timer2.Sequence() << "\n";
    Timer timer3(callback3, stamp3, 0);
    cout << "CreateNum" << timer3.Sequence() << "\n";
    timer1.Run();
    timer2.Run();
    timer3.Run();
}

int main() {
    // TimeStamp_t();
    // Clock_t();
    Timer_t();
}