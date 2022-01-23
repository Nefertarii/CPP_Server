#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "deque.hpp"
#include "queue.hpp"
#include "vector.hpp"
#include <thread>
#include <functional>
#include <exception>
#include <future>
#include <iostream>

namespace Thread {
    class Thread_Pool {
    private:
        struct Impl_base {
            virtual void call() = 0;
            virtual ~Impl_base() {}
        };
        template <typename Func>
        struct Impl_type : Impl_base {
            Func func;
            Impl_type(Func&& func_) :func(std::move(func_)) {}
            void call() { func(); }
        };
        class Function_wrap {
        private:
            std::unique_ptr<Impl_base> impl;
        public:
            Function_wrap() = default;
            template <typename Func>
            Function_wrap(Func&& func) :impl(new Impl_type<Func>(std::move(func))) {}
            void operator()() { impl->call(); }
            Function_wrap(Function_wrap&& other) :impl(std::move(other.impl)) {}
            Function_wrap& operator=(Function_wrap&& other);
            Function_wrap(const Function_wrap&) = delete;
            Function_wrap(Function_wrap&) = delete;
            Function_wrap& operator=(const Function_wrap&) = delete;
        };
        using Task = Function_wrap;
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
        class Join_threads {
        private:
            std::vector<std::thread>& threads;
        public:
            explicit Join_threads(std::vector<std::thread>& threads_) :
                threads(threads_) {}
            ~Join_threads();
        };
        std::atomic_bool done;
        Thread::Safe_Queue<Task> pool_work_queue;
        std::vector<std::unique_ptr<Task_queue>> tasks;
        std::vector<std::thread> threads;
        Join_threads joiner;
        inline static thread_local Task_queue* local_work_queue;
        inline static thread_local int index;
        void worker_thread(uint index_);
        bool pop_task_from_local_queue(Task& task);
        bool pop_task_from_pool_queue(Task& task);
        bool pop_task_from_other_thread_queue(Task& task);
    public:
        Thread_Pool();
        template <typename Func>
        std::future<typename std::result_of<Func()>::type> submit(Func func);
        void run_pending_task();
        ~Thread_Pool() { done = true; }
    };
};

Thread::Thread_Pool::Function_wrap& Thread::Thread_Pool::Function_wrap::operator=(Function_wrap&& other) {
    impl = std::move(other.impl);
    return *this;
}

void Thread::Thread_Pool::Task_queue::push(Task data) {
    std::lock_guard<std::mutex> lk(mtx);
    deque.push_front(std::move(data));
}

bool Thread::Thread_Pool::Task_queue::empty() const {
    std::lock_guard<std::mutex> lk(mtx);
    return deque.empty();
}

bool Thread::Thread_Pool::Task_queue::try_pop(Task& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) { return false; }
    else {
        res = std::move(deque.front());
        deque.pop_front();
        return true;
    }
}

bool Thread::Thread_Pool::Task_queue::try_steal_task(Task& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) { return false; }
    else {
        res = std::move(deque.back());
        deque.pop_back();
        return true;
    }
}

Thread::Thread_Pool::Join_threads::~Join_threads() {
    for (uint i = 0;i < threads.size();i++) {
        if (threads[i].joinable()) { threads[i].join(); }
    }
}

void Thread::Thread_Pool::worker_thread(uint index_) {
    index = index_;
    local_work_queue = tasks[index].get();
    while (!done) { run_pending_task(); }
}

Thread::Thread_Pool::Thread_Pool() :done(false), joiner(threads) {
    const uint thread_count = std::thread::hardware_concurrency();\
    try {
        for (uint i = 0;i < thread_count;i++) {
            tasks.push_back(std::unique_ptr<Task_queue>(new Task_queue));
            threads.push_back(std::thread(&Thread_Pool::worker_thread, this, i));
        }
    }
    catch (...) {
        done = true;
        throw;
    }
}

//thread_local Thread::Thread_Pool::Task_queue* Thread::Thread_Pool::local_work_queue = 0;
bool Thread::Thread_Pool::pop_task_from_local_queue(Task& task) {
    return local_work_queue && local_work_queue->try_pop(task);
}

bool Thread::Thread_Pool::pop_task_from_pool_queue(Task& task) {
    return pool_work_queue.try_pop(task);
}

//thread_local int Thread::Thread_Pool::index = 0;
bool Thread::Thread_Pool::pop_task_from_other_thread_queue(Task& task) {
    for (uint i = 0;i < tasks.size();i++) {
        const int new_index = (Thread::Thread_Pool::index + i + 1) % tasks.size();
        if (tasks[new_index]->try_steal_task(task)) { return true; }
    }
    return false;
}

template <typename Func>
std::future<typename std::result_of<Func()>::type> Thread::Thread_Pool::submit(Func func) {
    using ResultType = typename std::result_of<Func()>::type;
    std::packaged_task<ResultType()> task(func);
    std::future<ResultType> res(task.get_future());
    if (local_work_queue) { local_work_queue->push(std::move(task)); }
    else { pool_work_queue.push(std::move(task)); }
    return res;
}

void Thread::Thread_Pool::run_pending_task() {
    Task task;
    if (pop_task_from_local_queue(task) ||
        pop_task_from_pool_queue(task) ||
        pop_task_from_other_thread_queue(task)) {
        task();
    }
    else { std::this_thread::yield(); }
}

#endif