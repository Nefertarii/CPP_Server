#include "../Log/Head/logging.h"
#include <atomic>
#include <cstring>
#include <errno.h>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

class Thread {
private:
    bool started;
    bool joined;
    std::string name;
    std::function<void()> thread_func;
    // std::packaged_task<void> thread_func;
    pthread_t pid;
    pid_t tid;
    std::thread thread_core;
    static std::atomic<int> num_created;
    void Set_default_name();

public:
    explicit Thread(std::function<void()> func_, const std::string& name_ = std::string());
    void Start();
    int Join();
    bool Started() const;
    pid_t Get_tid() const;
    const std::string& Get_name() const;
    static int Num_created() { return num_created.load(); }
    ~Thread();
};

std::atomic<int> Thread::num_created;

Thread::Thread(std::function<void()> func, const std::string& name_) :
    started(false),
    joined(false),
    name(name_),
    thread_func(func),
    pid(0),
    tid(0),
    thread_core() {
    Set_default_name();
    std::string msg = "Thread:" + name + " Create\n";
    std::cout << msg;
}

void Thread::Set_default_name() {
    int num = num_created.fetch_add(1, std::memory_order_relaxed);
    if (name.empty()) {
        name = "thread_" + std::to_string(num);
    }
}

void Thread::Start() {
    if (started) {
        std::cout << "requie thread is started\n";
        return;
    }
    try {
        started = true;
        std::cout << "Thread:" + name + " start\n";
        thread_core = std::thread(thread_func);
    } catch (const std::exception& e) {
        started = false;
        std::cout << "thread:" + name + " create fail!\n";
    }
}

int Thread::Join() {
    if (!started) {
        std::cout << "thread:" + std::to_string(gettid()) + "not started\n";
        return -1;
    }
    if (joined) {
        std::cout << "thread:" + std::to_string(gettid()) + "is joined\n";
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
    std::cout << "Thread:" + name + " earse\n";
    if (started && !joined) {
        thread_core.detach();
    }
}

std::atomic<int> flag = 0;

void print() {
    while (flag != 1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "thread:" << gettid() << " hello.\n";
    }
    LOG_INFO("fetadd");
    flag.fetch_add(1);
}

void func2() {
    Thread t1(print);
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
    // func1();
}