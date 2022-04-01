#ifndef BASE_TASKQUEUE_H_
#define BASE_TASKQUEUE_H_

#include "../../Class/noncopyable.h"
#include <deque>
#include <functional>
#include <mutex>

namespace Wasi {
namespace Base {

using Task = std::function<void()>;

class TaskQueue : Noncopyable {
private:
    std::deque<Task> task_deque;
    mutable std::mutex mtx;

public:
    TaskQueue() = default;
    void push(Task task);
    bool empty() const;
    bool try_pop(Task& task);
    bool try_steal_task(Task& task);
};

}
} // namespace Wasi::Base

#endif // !BASE_TASKQUEUE_H_