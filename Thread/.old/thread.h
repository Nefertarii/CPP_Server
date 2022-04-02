#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_

#include "../../Class/noncopyable.h"
#include "latch.h"
#include <atomic>
#include <functional>
#include <string>
#include <thread>

namespace Wasi {
namespace Base {

using Function = std::function<void()>;

class Thread : Noncopyable {
private:
    void Set_default_name();
    bool started;
    bool joined;
    std::string name;
    Function func;
    Latch latch;
    pthread_t pthread_id;
    pid_t tid;
    static std::atomic<int> num_created;

public:
    explicit Thread(Function func_, const std::string& name_ = std::string());
    void Start();
    int Join();
    bool Started() const;
    pid_t Tid() const;
    const std::string& Name() const;
    static int Num_created() { return num_created.load(); }
    ~Thread();
};

#include <cassert>
#include <errno.h>
#include <exception>
#include <linux/unistd.h>
#include <memory>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

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
    thread_data->tid        = nullptr;
    thread_data->latch->Count_down();
    thread_data->latch = nullptr;
    prctl(PR_SET_NAME, thread_data->name);
    try {
        thread_data->func();
        thread_data->name = "Finished";
    } catch (...) {
        thread_data->name = "crashed";
    }
    delete thread_data;
    return nullptr;
}

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
    std::string msg = "Thread:" + name + " Create";
}

std::atomic<int> Thread::num_created(0);

void Thread::Start() {
    assert(!started);
    started          = true;
    ThreadData* data = new ThreadData(name, func, &latch, &tid);
    if (pthread_create(&pthread_id, nullptr, Start_thread, data)) {
        started = false;
        delete data;
    } else {
        latch.Wait();
        assert(tid > 0);
    }
}

int Thread::Join() {
    assert(started);
    assert(!joined);
    joined = true;
    return pthread_join(pthread_id, nullptr);
}

bool Thread::Started() const { return started; }

pid_t Thread::Tid() const { return tid; }

const std::string& Thread::Name() const { return name; }

Thread::~Thread() {
    if (started && !joined) {
        pthread_detach(pthread_id);
    }
}

}
} // namespace Wasi::Base

#endif // !BASE_THREAD_H_