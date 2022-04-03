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
using Task = std::function<void()>;

SafeQueue<Task> pool_work_queue;

void print(int num) {
    printf("tid:%d get task, num:%d\n", gettid(), num);
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

int main() {
    Wasi::Base::ThreadPool tp;
    this_thread::sleep_for(chrono::seconds(1));
    tp.Start();
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            tp.Stop();
        }
        if (i == 8) {
            tp.Start();
        }
        tp.Submit(std::bind(&print, i));
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}