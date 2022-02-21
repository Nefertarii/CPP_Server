#include "Head/thread.h"
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <linux/unistd.h>
#include <string>
#include <cassert>
#include <latch>
#include <memory>
#include <exception>
#include <iostream>

using namespace Wasi::Net;

struct ThreadData {
    std::string name;
    ThreadFunc func;
    Latch* latch;
    pid_t* tid;
    Thread(std::string name_, ThreadFunc func_, Latch* latch_, pid_t* tid_) :
        name(name_),
        func(std::move(func_)),
        latch(latch_),
        tid(tid_) {}
}

void* Start_thread(void* obj) {
    ThreadData* thread_data = static_cast<ThreadData*>(obj);
    thread_data->tid = gettid();
    //tid = nullptr;
    thread_data->latch->count_down();
    //thread_data->latch = nullptr;
    prctl(PR_SET_NAME, thread_data->name);
    try {
        thread_data->func();
        thread_data->name = "Finished";
    }
    catch (...) {
        thread_data->name = "crashed";
        std::cout<<"..."
    }

    
    delete thread_data;
    return nullptr;
}

void Thread::Set_default_name() {
    int num = num_created.fetch_add(1);
    if (name.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name = buf;
        std::cout << "thread name:" << name << "\n";
    }
}

Thread::Thread(ThreadFunc func_, const string& name_ = string()) :
    started(false),
    joined(false),
    pthread_id(0),
    tid(0),
    name(name_),
    func(std::move(func_)),
    latch(1) {
    Set_default_name();
}

void Thread::Start() {
    assert(!started);
    started = true;
    if (pthread_create(&pthread_id, nullptr, data))

}

int Thread::Join();

bool Thread::Started() const { return started; }

pid_t Thread::Tid() const { return tid; }

const string& Thread::Name() const { return name; }

static int Thread::Num_created() { return num_created.load(); }

Thread::~Thread() {
    if (started && !joined) {
        pthread_detach(pthread_id);
    }
}