#include "../Log/Head/logging.h"
#include "../Log/Sink/Head/filesink.h"
#include "../Net/Http/Head/httpserver.h"
#include "../Net/Poll/Head/eventloop.h"
#include "../Net/Poll/Head/eventloopthread.h"
#include "../Net/Poll/Head/eventloopthreadpool.h"
#include "../Thread/Head/threadpool.h"
#include "../Timer/Head/timerid.h"
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace Wasi;
using namespace Wasi::Base;
using namespace Wasi::Poll;
using namespace Wasi::Http;
using Task = std::function<void()>;

SafeQueue<Task> pool_work_queue;

void print1(EventLoop* eventloop = nullptr) {
    printf("tid:%d get task, num:1\n", gettid());
}
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
void while_print1(EventLoop* eventloop = nullptr) {
    int i = 5;
    while (--i) {
        printf("tid:%d get task, num:%d\n", gettid(), i);
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

    threadpool.Submit(std::bind(&EventLoop::Loop, loop));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop2));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop3));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop4));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop5));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop6));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop7));
    threadpool.Submit(std::bind(&EventLoop::Loop, loop8));

    threadpool.Start();
}
void func4() {
    EventLoop loop;
    loop.Run_after(3, std::bind(&EventLoop::Quit, &loop));
    EventLoopThreadPool loop_threadpool(&loop, "threadpool");
    loop_threadpool.Set_thread_num(4);
    loop_threadpool.Start(print1);
    EventLoop* Loop1 = loop_threadpool.Get_loop();
    EventLoop* Loop2 = loop_threadpool.Get_loop();
    EventLoop* Loop3 = loop_threadpool.Get_loop();
    EventLoop* Loop4 = loop_threadpool.Get_loop();
    Loop1->Run_in_loop(std::bind(while_print, 1));
    Loop2->Run_in_loop(std::bind(while_print, 2));
    Loop3->Run_in_loop(std::bind(while_print, 3));
    Loop4->Run_in_loop(std::bind(while_print, 4));
}
class Class1 {
private:
    int i = 0;

public:
    void Add() {
        ++i;
        printf("tid:%d get task, num:%d\n", gettid(), i);
    };
    void Print() { cout << i << "\n"; }
};
void func5() {
    Class1 T1;
    ThreadPool tp;
    tp.Start();
    tp.Submit(std::bind(&Class1::Add, &T1));
    tp.Submit(std::bind(&Class1::Add, &T1));
    tp.Submit(std::bind(&Class1::Add, &T1));
    tp.Submit(std::bind(&Class1::Add, &T1));
    this_thread::sleep_for(chrono::seconds(1));
    T1.Print();
}
void func6() {
    Wasi::Log::Change_default_logger(std::make_shared<Log::FileSink>("test.log"));
    EventLoop server_loop;
    Sockets::InetAddress linsten("127.0.0.1", 8000);
    HttpServer httpserver(&server_loop, linsten, "httpserver");
    httpserver.Start();
    server_loop.Loop();
}

std::atomic<int> flag = 0;

void print2(int n) {
    while (flag != 1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "thread:" << gettid() << " hello." << n << "\n";
    }
    LOG_INFO("fetadd");
    flag.fetch_add(1);
}

void func7() {
    Thread t1(std::bind(print2, 2));
    t1.Start();
    std::cout << flag.load() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    LOG_INFO("fetadd");
    flag.fetch_add(1);
    std::cout << flag.load() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << flag.load() << "\n";
}

int main() {
    func7();
}