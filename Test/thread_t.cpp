#include "Include/Header/thread.h"
#include "Include/Timer/clock.h"
#include "Include/Header/latch.h"
#include <iostream>
#include <unistd.h>
#include <exception>

using namespace wasi;
using namespace wasi::head;

head::Latch global_latch(3);

void print(int j) {
    for (int i = 0; i < 5; i++) {
        // std::cout <<"print:" << j << ", in thread:" << gettid() << "\n";
        std::cout << j;
        std::this_thread::sleep_for(timer::Timer_Ms(800 * j));
    }
    std::cout << "print done, in:" << timer::Clock::NowtimeSec() << "\n";
    global_latch.CountDown();
}

void Thread_t() {
    int num = 1;

    Thread t1(std::bind(print, num++));
    t1.Start();
    std::cout << "main thread:" << gettid() << "\n";
    std::cout << "create thread:" << t1.GetName() << ", in:" << timer::Clock::NowtimeSec() << "\n";

    Thread t2(std::bind(print, num++));
    t2.Start();
    std::cout << "create thread:" << t2.GetName() << ", in:" << timer::Clock::NowtimeSec() << "\n";

    Thread t3(std::bind(print, num++));
    t3.Start();
    std::cout << "create thread:" << t3.GetName() << ", in:" << timer::Clock::NowtimeSec() << "\n";
    global_latch.Wait();
    std::cout << "all done in:" << timer::Clock::NowtimeSec();
    std::cout << "\n";
}

int main() {
    Thread_t();
}