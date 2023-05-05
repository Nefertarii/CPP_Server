#include "thread.h"
#include <Log/logging.h>
#include <cstring>
#include <errno.h>
#include <exception>
#include <memory>
#include <unistd.h>

using namespace Wasi;
using namespace Wasi::Base;

std::atomic<int> Thread::num_created;

void Thread::Set_default_name() {
    int num = num_created.fetch_add(1, std::memory_order_relaxed);
    if (name.empty()) {
        name = "thread_" + std::to_string(num);
    }
}

Thread::Thread(Function func, const std::string& name_) :
    started(false),
    joined(false),
    name(name_),
    thread_func(func),
    pid(0),
    tid(0),
    thread_core() {
    Set_default_name();
    std::string msg = "Thread:" + name + " create";
    LOG_INFO(msg);
}

void Thread::Start() {
    if (started) {
        LOG_ERROR("requie thread is started");
        return;
    }
    try {
        started     = true;
        thread_core = std::thread(thread_func);
    } catch (const std::exception& e) {
        started = false;
        LOG_FATAL("thread:" + name + " create fail!");
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
    thread_core.join();
    return 0;
}

bool Thread::Started() const { return started; }

pid_t Thread::Get_tid() const { return tid; }

const std::string& Thread::Get_name() const { return name; }

Thread::~Thread() {
    LOG_INFO("Thread:" + name + " detach");
    if (started && !joined) {
        thread_core.detach();
    }
}