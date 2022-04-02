#include "Head/threadpool.h"
#include "../Class/exception.h"
#include "../Log/Head/logging.h"
#include <exception>

using namespace Wasi::Base;

void ThreadPool::Worker_thread(uint index_) {
    index            = index_;
    local_work_queue = tasks[index].get();
    while (!done) { Run_pending_task(); }
}

// thread_local Wasi::Thread::Thread_Pool::Task_queue* Wasi::Thread::Thread_Pool::local_work_queue = 0;
// thread_local int Wasi::Thread::Thread_Pool::index = 0;
bool ThreadPool::Poptask_from_local_queue(Task& task_) {
    return local_work_queue && local_work_queue->Try_pop(task_);
}

bool ThreadPool::Poptask_from_pool_queue(Task& task_) {
    return pool_work_queue.Try_pop(task_);
}

bool ThreadPool::Poptask_from_other_thread_queue(Task& task_) {
    for (uint i = 0; i < tasks.size(); i++) {
        const size_t new_index = (ThreadPool::index + i + 1) % tasks.size();
        if (tasks[new_index]->Try_steal_task(task_)) { return true; }
    }
    return false;
}

ThreadPool::ThreadPool() :
    done(false),
    pool_work_queue(),
    tasks(),
    threads() {
    const uint thread_count = std::thread::hardware_concurrency();
    tasks.reserve(thread_count);
    threads.reserve(thread_count);
    try {
        for (uint i = 0; i < thread_count; i++) {
            tasks.emplace_back(std::make_unique<TaskQueue>());
            std::string thread_name = "thread" + std::to_string(i);
            threads.emplace_back(std::make_unique<Thread>(
                std::bind(&ThreadPool::Worker_thread, this, i), thread_name));
        }
    } catch (const std::exception& e) {
        e.what();
        // LOG_CRITICAL();
        done = true;
    }
}

void ThreadPool::Submit(Task task_) {
    if (local_work_queue) {
        local_work_queue->Push(std::move(task_));
    } else {
        pool_work_queue.Push(std::move(task_));
    }
}

void ThreadPool::Run_pending_task() {
    for (uint i = 0; i < threads.size(); i++) {
        threads[i]->Start();
    }
    Task task;
    if (Poptask_from_local_queue(task) || Poptask_from_pool_queue(task)) {
        task();
    } else {
        std::this_thread::yield();
    }
}

ThreadPool::~ThreadPool() {
    done = true;
}