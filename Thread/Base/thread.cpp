#include "Head/thread.h"
#include "../../Log/Head/logging.h"
#include <cstring>
#include <errno.h>
#include <exception>
#include <linux/unistd.h>
#include <memory>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

using namespace Wasi;
using namespace Wasi::Base;

struct ThreadData {
    std::string name;
    Function func;
    Latch* latch;
    pid_t* tid;
    ThreadData(std::string name_, Function func_, Latch* latch_, pid_t* tid_) :
        name(name_),
        func(std::move(func_)),
        latch(latch_),
        tid(tid_) {}
};

void* Start_thread(void* obj) {
    ThreadData* thread_data = static_cast<ThreadData*>(obj);
    *(thread_data->tid)     = gettid();
    LOG_INFO("thread:" + std::to_string(*thread_data->tid) + " start");
    thread_data->tid = nullptr;
    thread_data->latch->Count_down();
    thread_data->latch = nullptr;

    prctl(PR_SET_NAME, thread_data->name);
    try {
        thread_data->func();
        thread_data->name = "Finished";
    } catch (...) {
        thread_data->name = "Crashed";
    }
    delete thread_data;
    return nullptr;
}

std::atomic<int> Thread::num_created;

void Thread::Set_default_name() {
    int num = num_created.fetch_add(1, std::memory_order_relaxed);
    if (name.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name = buf;
    }
}

Thread::Thread(Function func_, const std::string& name_) :
    started(false),
    joined(false),
    name(name_),
    func(std::move(func_)),
    latch(1),
    pthread_id(0),
    tid(0) {
    Set_default_name();
    std::string msg = "Thread: " + name + " Create";
    LOG_INFO(msg);
}

void Thread::Start() {
    if (started) {
        LOG_ERROR("requie thread is started");
        return;
    }
    started          = true;
    ThreadData* data = new ThreadData(name, func, &latch, &tid);
    if (pthread_create(&pthread_id, nullptr, Start_thread, data)) {
        started = false;
        delete data;
        LOG_ERROR("pthread_create fail" + std::string(strerror(errno)));
    } else {
        latch.Wait();
        if (tid <= 0) {
            LOG_ERROR("requie thread is quit");
        }
    }
}

int Thread::Join() {
    if (!started) {
        LOG_ERROR("thread:" + std::to_string(gettid()) + "not started");
        return -1;
    }
    if (joined) {
        LOG_ERROR("thread:" + std::to_string(gettid()) + "is joined");
        return -1;
    }
    joined = true;
    return pthread_join(pthread_id, nullptr);
}

bool Thread::Started() const { return started; }

pid_t Thread::Get_tid() const { return tid; }

const std::string& Thread::Get_name() const { return name; }

Thread::~Thread() {
    LOG_INFO("Thread " + std::to_string(tid) + " earse");
    if (started && !joined) {
        pthread_detach(pthread_id);
    }
}