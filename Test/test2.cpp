#include <iostream>
#include <bitset>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <functional>
#include <thread>

#include <algorithm>
#include <sys/epoll.h>
#include <unistd.h>

//#include "../Headfile/Clock/Head/timer.h"

class Timer2 {
public:
    Timer2(int rotations, int slot, std::function<void(void)> fun, void* args)
        : rotations_(rotations), slot_(slot), fun(fun) {}
    inline int getRotations() { return rotations_; }
    inline void decreaseRotations() { --rotations_; }
    inline void active() { fun(); }
    inline int getSlot() { return slot_; }
private:
    int rotations_;
    int slot_;
    std::function<void(void)> fun;
    void* args;
};

class TimeWheel {
public:
    TimeWheel(int nslots)
        : nslosts_(nslots), curslot_(0),
        slots_(nslosts_, std::vector<Timer2*>()), starttime_(getCurrentMillisecs()){}
        ~TimeWheel() {
        for (std::vector<Timer2*> vect : slots_)
            for (Timer2* timer : vect)
                delete timer;
    }
    unsigned long long getCurrentMillisecs() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
        return ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
    }
    Timer2* addTimer(unsigned long long timeout, std::function<void(void)> fun, void* args) {
        int slot = 0;
        Timer2* timer = NULL;
        if (timeout < 0)
            return NULL;
        slot = (curslot_ + (timeout % nslosts_)) % nslosts_;
        timer = new Timer2(timeout / nslosts_, slot, fun, args);
        slots_[slot].push_back(timer);
        return timer;
    }
    void delTimer(Timer2* timer) {
        if (!timer)
            return;
        std::vector<Timer2*>::iterator it = std::find(slots_[timer->getSlot()].begin(), slots_[timer->getSlot()].end(), timer);
        if (it != slots_[timer->getSlot()].end()) {
            slots_[timer->getSlot()].erase(it);
            //delete timer;
        }
    }
    void tick() {
        for (auto it = slots_[curslot_].begin(); it != slots_[curslot_].end();) {
            if ((*it)->getRotations() > 0) {
                (*it)->decreaseRotations();
                ++it;
                continue;
            }
            else {
                Timer2* timer = *it;
                timer->active();
                it = slots_[curslot_].erase(it);
                //delete timer
            }
        }
        curslot_ = ++curslot_ % nslosts_;
    }
    void takeAllTimeout() {
        int now = getCurrentMillisecs();
        int cnt = now - starttime_;
        for (int i = 0; i < cnt; ++i)
            tick();
        starttime_ = now;
    }
private:
    int nslosts_;
    int curslot_;
    unsigned long long starttime_;
    std::vector<std::vector<Timer2*>> slots_;
};

void func1() { std::cout << "hello world" << std::endl; }
void func2() { std::cout << "hello world2" << std::endl; }



using Ulong = unsigned long;
using Us = std::chrono::microseconds;
using Ms = std::chrono::milliseconds;
using Sec = std::chrono::seconds;
using SysClock = std::chrono::system_clock;
using HighResClock = std::chrono::high_resolution_clock;
using SteadyClock = std::chrono::steady_clock;
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;
template <typename type>
using Time = std::chrono::time_point<HighResClock, type>;

class Timer {
private:
    Ms create_time_ms;
    Sec create_time_sec;
    SteadyClock clock;
    Ms Now_time_ms_();
    Sec Now_time_sec_();
public:
    Timer();
    long Now_time_ms();
    long Now_time_sec();
    long Run_time_ms();
    long Run_time_sec();
    std::string Sec_to_string(long time);
    //Set_time();
    void Reset();
    ~Timer() {};
};

Ms Timer::Now_time_ms_() {
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(HighResClock::now());
    return now_ms.time_since_epoch();
}

Sec Timer::Now_time_sec_() {
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(HighResClock::now());
    return now_sec.time_since_epoch();
}

Timer::Timer() {
    create_time_ms = Now_time_ms_();
    create_time_sec = Now_time_sec_();
}

long Timer::Now_time_ms() {
    Time<Ms> now_ms = std::chrono::time_point_cast<Ms>(HighResClock::now());
    return now_ms.time_since_epoch().count();
}

long Timer::Now_time_sec() {
    Time<Sec> now_sec = std::chrono::time_point_cast<Sec>(HighResClock::now());
    return now_sec.time_since_epoch().count();
}

long Timer::Run_time_ms() {
    Ms time_ms = Now_time_ms_();
    return time_ms.count() - create_time_ms.count();
}

long Timer::Run_time_sec() {
    Sec time_sec = Now_time_sec_();
    return time_sec.count() - create_time_sec.count();
}

std::string Timer::Sec_to_string(long time) {
    char temp[80];
    struct tm *time_tm = localtime(&time);
    strftime(temp, 80, "Data:%a, %b %m %Y %H:%M:%S GMT\n", time_tm);
    std::string str = temp;
    return str;
}

void Timer::Reset() {
    create_time_ms = Now_time_ms_();
    create_time_sec = Now_time_sec_();
}

TimeWheel tw(60 * 1000);
Timer clock2;

int dispatch()
{
    tw.addTimer(1000, func1, NULL);
    tw.addTimer(2000, clock2.Now_time_ms, nullptr);
    tw.addTimer(5000, []() { std::cout << "hello baixiancpp" << std::endl; }, NULL);
    for(;;)
    {
        //int ret = epoll_wait(epollfd, events, events_num, 1); // 基准频率 1ms
        // tw.tick(); 不要这么做，会导致误差累积
        tw.takeAllTimeout();
    }
}

int main() {
    std::thread T1(dispatch);
    T1.detach();
    //tw.addTimer(100, func1, nullptr);
    tw.addTimer(2000, clock.Now_time_ms, nullptr);
    sleep(5);
    //dispatch();
}