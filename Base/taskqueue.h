#ifndef BASE_TASKQUEUE_H_
#define BASE_TASKQUEUE_H_

#include <deque>
#include <functional>
#include <future>
#include <mutex>

namespace Wasi {
namespace Base {
using Task = std::function<void()>;

class TaskQueue {
private:
    std::deque<Task> task_deque;
    mutable std::mutex mtx;

public:
    TaskQueue() {}
    void Push(Task task);
    TaskQueue(const TaskQueue&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;
    bool Empty() const;
    bool Try_pop(Task& task);
    bool Try_steal_task(Task& task);
};

}
} // namespace Wasi::Base

#endif // !BASE_TASKQUEUE_H_