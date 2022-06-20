#include "threadpool.h"
#include <Log/logging.h>
#include <Base/exception.h>
#include <exception>
#include <unistd.h>

using namespace Wasi::Base;

void ThreadPool::Worker_thread(uint index_) {
    index            = index_;
    local_work_queue = tasks[index].get();
    while (!done) {
        Task task;
        if (Poptask_from_local_queue(task)
            || Poptask_from_pool_queue(task)
            || Poptask_from_other_thread_queue(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
    LOG_INFO("thread stop");
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
    thread_count(std::thread::hardware_concurrency()),
    mtx(),
    done(false),
    pool_work_queue(),
    tasks(),
    threads() {}

void ThreadPool::Submit(Task task_) {
    if (local_work_queue) {
        local_work_queue->Push(std::move(task_));
    } else {
        pool_work_queue.Push(std::move(task_));
    }
}

void ThreadPool::Start() {
    tasks.reserve(thread_count);
    threads.reserve(thread_count);
    done = false;
    try {
        for (int i = 0; i < thread_count; i++) {
            tasks.emplace_back(std::make_unique<TaskQueue>());
            std::string thread_name = "thread" + std::to_string(i);
            threads.emplace_back(std::make_unique<Thread>(
                std::bind(&ThreadPool::Worker_thread, this, i), thread_name));
            threads[i]->Start();
        }
        LOG_INFO("thread pool start");
    } catch (const std::exception& e) {
        e.what();
        // LOG_CRITICAL();
        done = true;
    }
}

void ThreadPool::Stop() {
    LOG_INFO("thread pool stop");
    if (threads.empty()) { return; }
    {
        std::lock_guard<std::mutex> lk(mtx);
        done = true;
        pool_work_queue.Notifi_all();
    }
    for (int i = thread_count - 1; i >= 0; --i) {
        int pid = threads[i]->Get_tid();
        int ret = threads[i]->Join();
        if (ret == 0) {
            LOG_INFO("thread:" + std::to_string(pid) + " success quit");
            threads.erase(threads.begin() + i);
        } else if (ret < 0) {
            LOG_CRITICAL("thread:" + std::to_string(pid) + " join unknow error\n");
        } else {
            LOG_CRITICAL("thread:" + std::to_string(pid) + " not quit " + std::string(strerror(errno)));
        }
    }
}

void ThreadPool::Set_thread_count(int num) {
    if (num > 0) {
        Stop();
        thread_count = num;
        Start();
    }
}

ThreadPool::~ThreadPool() {
    if (done == false) {
        Stop();
    }
}