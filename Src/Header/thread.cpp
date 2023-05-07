#include "Include/Header/thread.h"
#include <cstring>
#include <memory>
#include <unistd.h>

using namespace wasi;
using namespace wasi::head;

std::atomic<int> Thread::num_created_ = 0;

void Thread::SetDefaultName() {
    int num = num_created_.fetch_add(1, std::memory_order_relaxed);
    if (name_.empty()) {
        name_ = "thread#" + std::to_string(num);
    }
}

Thread::Thread(Function func, const std::string& name) :
    started_(false),
    joined_(false),
    name_(name),
    thread_func_(func),
    pid_(0),
    tid_(0),
    thread_core_() {
    SetDefaultName();
    // std::string msg = name + " create";
}

void Thread::Start() {
    if (started_) {
        return;
    }
    started_     = true;
    thread_core_ = std::thread(thread_func_);
}

int Thread::Join() {
    if (started_ || !joined_) {
        return -1;
    }
    joined_ = true;
    thread_core_.join();
    return 0;
}

bool Thread::Started() const { return started_; }

pid_t Thread::GetTid() const { return tid_; }

const std::string& Thread::GetName() const { return name_; }

int Thread::NumCreated() { return num_created_.load(); }

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_core_.detach();
    }
}