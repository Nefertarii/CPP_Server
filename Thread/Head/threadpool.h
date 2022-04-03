#ifndef THREAD_THREADPOOL_H_
#define THREAD_THREADPOOL_H_

#include "../Base/Head/safequeue.h"
#include "../Base/Head/taskqueue.h"
#include "../Base/Head/thread.h"
#include <iostream>
#include <thread>

namespace Wasi {
namespace Base {

class ThreadPool {
private:
    int thread_count;
    mutable std::mutex mtx;
    std::atomic<bool> done;
    SafeQueue<Task> pool_work_queue;
    std::vector<std::unique_ptr<TaskQueue>> tasks;
    std::vector<std::unique_ptr<Thread>> threads;
    inline static thread_local TaskQueue* local_work_queue;
    inline static thread_local int index;
    void Worker_thread(uint index_);
    bool Poptask_from_local_queue(Task& task_);
    bool Poptask_from_pool_queue(Task& task_);
    bool Poptask_from_other_thread_queue(Task& task_);

public:
    ThreadPool();
    void Submit(Task task_);
    void Start();
    void Stop();
    ~ThreadPool();
};

}
} // namespace Wasi::Base

#endif // !THREAD_THREADPOOL_H_