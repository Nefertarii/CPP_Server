#include "Include/Eventloop/eventloopthreadpool.h"
#include "Include/Eventloop/eventloopthread.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Header/thread.h"
#include "Include/Header/latch.h"
#include "Include/Timer/clock.h"

#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace wasi;
using namespace wasi::loop;

void print(EventLoop* p = NULL) {
    printf("print: pid = %d, tid = %d, loop = %p\n",
           getpid(), gettid(), p);
}

void quit(EventLoop* p) {
    print(p);
    p->Quit();
}

void init(EventLoop* p) {
    printf("init(): pid = %d, tid = %d, loop = %p\n",
           getpid(), gettid(), p);
}

void EvnetLoopThread_t() {
    print();

    EventLoop loop;
    loop.RunAfter(11, std::bind(&EventLoop::Quit, &loop));

    {
        printf("Single thread %p:\n", &loop);
        EventLoopThreadPool model(&loop, "single");
        model.SetThreadNum(0);
        model.Start(init);
        assert(model.GetLoop() == &loop);
        assert(model.GetLoop() == &loop);
        assert(model.GetLoop() == &loop);
    }

    {
        printf("Another thread:\n");
        EventLoopThreadPool model(&loop, "another");
        model.SetThreadNum(1);
        model.Start(init);
        EventLoop* nextLoop = model.GetLoop();
        nextLoop->RunAfter(2, std::bind(print, nextLoop));
        assert(nextLoop != &loop);
        assert(nextLoop == model.GetLoop());
        assert(nextLoop == model.GetLoop());
        ::sleep(3);
    }

    {
        printf("Three threads:\n");
        EventLoopThreadPool model(&loop, "three");
        model.SetThreadNum(3);
        model.Start(init);
        EventLoop* nextLoop = model.GetLoop();
        nextLoop->RunInLoop(std::bind(print, nextLoop));
        assert(nextLoop != &loop);
        assert(nextLoop != model.GetLoop());
        assert(nextLoop != model.GetLoop());
        assert(nextLoop == model.GetLoop());
    }

    loop.Loop();
}

void EvnetLoopThreadPool_t() {
    print();

    {
        EventLoopThread thr1; // never start
    }

    {
        // dtor calls quit()
        EventLoopThread thr2;
        EventLoop* loop = thr2.StartLoop();
        loop->RunInLoop(std::bind(print, loop));
        std::this_thread::sleep_for(timer::Timer_Ms(1000));
    }

    {
        // quit() before dtor
        EventLoopThread thr3;
        EventLoop* loop = thr3.StartLoop();
        loop->RunInLoop(std::bind(quit, loop));
        std::this_thread::sleep_for(timer::Timer_Ms(1000));
    }
}

int main() {
    EvnetLoopThread_t();
    //EvnetLoopThreadPool_t();
    std::cout << "\n";
}