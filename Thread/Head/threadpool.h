#ifndef THREAD_THREADPOOL_H_
#define THREAD_THREADPOOL_H_

#include "../Base/Head/queue.h"
#include "../Base/Head/taskqueue.h"
#include "../Base/Head/thread.h"
#include <future>
#include <iostream>
#include <thread>

namespace Wasi {
namespace Thread {

class ThreadPool {
private:
    std::atomic<bool> done;
    Base::SafeQueue<Base::Task> pool_work_queue;
    std::vector<std::unique_ptr<Base::TaskQueue>> tasks;
    std::vector<Base::Thread> threads;
    inline static thread_local Base::TaskQueue* local_work_queue;
    inline static thread_local int index;
    void Worker_thread(uint index_);
    bool Poptask_from_local_queue(Base::Task& task);
    bool Poptask_from_pool_queue(Base::Task& task);
    bool Poptask_from_other_thread_queue(Base::Task& task);

public:
    ThreadPool();
    std::future<std::result_of<Base::Task()>::type> Submit(Base::Task task);
    void Run_pending_task();
    ~ThreadPool();
};

}
} // namespace Wasi::Thread

#endif // !THREAD_THREADPOOL_H_