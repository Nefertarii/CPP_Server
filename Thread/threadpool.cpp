#include "Head/threadpool.h"
#include "../Class/exception.h"
#include "../Log/Head/logging.h"

using namespace Wasi::Thread;
using namespace Wasi;

void ThreadPool::Worker_thread(uint index_) {
    index            = index_;
    local_work_queue = tasks[index];
    while (!done) { Run_pending_task(); }
}

// thread_local Wasi::Thread::Thread_Pool::Task_queue* Wasi::Thread::Thread_Pool::local_work_queue = 0;
// thread_local int Wasi::Thread::Thread_Pool::index = 0;
bool ThreadPool::Poptask_from_local_queue(Base::Task& task) {
    return local_work_queue && local_work_queue->try_pop(task);
}

bool ThreadPool::Poptask_from_pool_queue(Base::Task& task) {
    return pool_work_queue.Try_pop(task);
}

bool ThreadPool::Poptask_from_other_thread_queue(Base::Task& task) {
    for (uint i = 0; i < tasks.size(); i++) {
        const size_t new_index = (ThreadPool::index + i + 1) % tasks.size();
        if (tasks[new_index]->try_steal_task(task)) { return true; }
    }
    return false;
}

ThreadPool::ThreadPool() :
    done(false),
    pool_work_queue(),
    tasks(),
    threads(), {
    const uint thread_count = std::thread::hardware_concurrency();
    try {
        for (uint i = 0; i < thread_count; i++) {
            tasks.push_back(std::make_unique<TaskQueue>());
            threads.push_back(Base::Thread(std::bind(Worker_thread, this, i),
                                           "thread" + std::to_string(i)));
        }
    } catch (const Exception& e) {
        LOG_CRITICAL(e.What());
        done = true;
    }
}

std::future<std::result_of<Base::Task()>::type> ThreadPool::Submit(Task task) {
}

void ThreadPool::Run_pending_task() {
}

ThreadPool::~ThreadPool() {
}