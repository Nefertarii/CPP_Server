#include "../Net/Poll/Head/eventloop.h"
#include "../Net/Poll/Head/eventloopthread.h"
#include "../Net/Poll/Head/eventloopthreadpool.h"
#include "../Thread/Head/threadpool.h"
//#include "../Thread/.old/threadpool.hpp"
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace Wasi::Base;
using namespace Wasi::Poll;
using Task = std::function<void()>;

SafeQueue<Task> pool_work_queue;

void print(int num) {
    printf("tid:%d get task, num:%d\n", gettid(), num);
}
void while_print(int num) {
    int i = 5;
    while (--i) {
        printf("tid:%d get task, num:%d\n", gettid(), num);
        this_thread::sleep_for(chrono::seconds(2));
    }
}
void func1() {
    this_thread::sleep_for(chrono::seconds(1));
    while (1) {
        Task task = pool_work_queue.Wait_pop();
        task();
    }
}
void func2() {
    // static thread_local TaskQueue* local_work_queue;
    std::vector<std::unique_ptr<Thread>> threads;
    // threads.reserve(4);
    for (int i = 0; i < 4; i++) {
        threads.push_back(std::make_unique<Thread>(func1));
    }
    for (uint i = 0; i < threads.size(); i++) {
        printf("start thread:%d\n", i);
        threads[i]->Start();
    }
    for (uint i = 0; i < 5; i++) {
        printf("push 1 task\n");
        pool_work_queue.Push(std::bind(print, i));
        this_thread::sleep_for(chrono::seconds(1));
    }
}
void func3() {
    ThreadPool threadpool;
    vector<unique_ptr<EventLoopThread>> loops;

    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());
    loops.push_back(make_unique<EventLoopThread>());

    EventLoop* loop  = loops[0]->Start_loop();
    EventLoop* loop2 = loops[1]->Start_loop();
    EventLoop* loop3 = loops[2]->Start_loop();
    EventLoop* loop4 = loops[3]->Start_loop();
    EventLoop* loop5 = loops[4]->Start_loop();
    EventLoop* loop6 = loops[5]->Start_loop();
    EventLoop* loop7 = loops[6]->Start_loop();
    EventLoop* loop8 = loops[7]->Start_loop();

    // threadpool.Submit(std::bind(&EventLoop::Loop, loop));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop2));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop3));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop4));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop5));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop6));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop7));
    // threadpool.Submit(std::bind(&EventLoop::Loop, loop8));

    threadpool.Start();
}

int main() {
    func3();
}