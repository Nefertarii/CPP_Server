#ifndef THREAD_THREADPOOL_H_
#define THREAD_THREADPOOL_H_

#include "queue.hpp"
#include "thread.h"
#include <deque>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <vector>

namespace Wasi {
namespace Base {

class Thread_Pool {
private:
    using Task = std::function<void()>;
    class Task_queue {
    private:
        std::deque<Task> deque;
        mutable std::mutex mtx;

    public:
        Task_queue() {}
        Task_queue(const Task_queue& other) = delete;
        Task_queue& operator=(const Task_queue& other) = delete;
        void push(Task data);
        bool empty() const;
        bool try_pop(Task& task);
        bool try_steal_task(Task& task);
    };
    std::atomic_bool done;
    Wasi::Base::Safe_Queue<Task> pool_work_queue;
    std::vector<std::unique_ptr<Task_queue>> tasks;
    std::vector<std::unique_ptr<Thread>> threads;
    inline static thread_local Task_queue* local_work_queue;
    inline static thread_local int index;
    void worker_thread(uint index_);
    bool pop_task_from_local_queue(Task& task);
    bool pop_task_from_pool_queue(Task& task);
    bool pop_task_from_other_thread_queue(Task& task);

public:
    Thread_Pool();
    void submit(Task func);
    void run_pending_task();
    ~Thread_Pool() { done = true; }
};

}
} // namespace Wasi::Base

void Wasi::Base::Thread_Pool::Task_queue::push(Task data) {
    std::lock_guard<std::mutex> lk(mtx);
    deque.push_front(std::move(data));
}

bool Wasi::Base::Thread_Pool::Task_queue::empty() const {
    std::lock_guard<std::mutex> lk(mtx);
    return deque.empty();
}

bool Wasi::Base::Thread_Pool::Task_queue::try_pop(Task& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) {
        return false;
    } else {
        res = std::move(deque.front());
        deque.pop_front();
        return true;
    }
}

bool Wasi::Base::Thread_Pool::Task_queue::try_steal_task(Task& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) {
        return false;
    } else {
        res = std::move(deque.back());
        deque.pop_back();
        return true;
    }
}

void Wasi::Base::Thread_Pool::worker_thread(uint index_) {
    index            = index_;
    local_work_queue = tasks[index].get();
    while (!done) { run_pending_task(); }
}

Wasi::Base::Thread_Pool::Thread_Pool() :
    done(false) {
    const uint thread_count = std::thread::hardware_concurrency();
    tasks.reserve(thread_count);
    threads.reserve(thread_count);
    try {
        for (uint i = 0; i < thread_count; i++) {
            tasks.emplace_back(std::make_unique<Task_queue>());
            std::string thread_name = "thread" + std::to_string(i);
            threads.emplace_back(std::make_unique<Thread>(
                std::bind(&Thread_Pool::worker_thread, this, i), thread_name));
        }
    } catch (...) {
        done = true;
        throw;
    }
}

// thread_local Wasi::Base::Thread_Pool::Task_queue* Wasi::Base::Thread_Pool::local_work_queue = 0;
bool Wasi::Base::Thread_Pool::pop_task_from_local_queue(Task& task) {
    return local_work_queue && local_work_queue->try_pop(task);
}

bool Wasi::Base::Thread_Pool::pop_task_from_pool_queue(Task& task) {
    return pool_work_queue.try_pop(task);
}

// thread_local int Wasi::Base::Thread_Pool::index = 0;
bool Wasi::Base::Thread_Pool::pop_task_from_other_thread_queue(Task& task) {
    for (uint i = 0; i < tasks.size(); i++) {
        const size_t new_index = (Wasi::Base::Thread_Pool::index + i + 1) % tasks.size();
        if (tasks[new_index]->try_steal_task(task)) { return true; }
    }
    return false;
}

void Wasi::Base::Thread_Pool::submit(Task task_) {
    if (local_work_queue) {
        local_work_queue->push(std::move(task_));
    } else {
        pool_work_queue.push(std::move(task_));
    }
}

void Wasi::Base::Thread_Pool::run_pending_task() {
    Task task;
    if (pop_task_from_local_queue(task) || pop_task_from_pool_queue(task) || pop_task_from_other_thread_queue(task)) {
        task();
    } else {
        std::this_thread::yield();
    }
}

#endif