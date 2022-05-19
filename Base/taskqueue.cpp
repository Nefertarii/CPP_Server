#include "taskqueue.h"

using namespace Wasi::Base;

void TaskQueue::Push(Task task) {
    std::lock_guard<std::mutex> lk(mtx);
    task_deque.push_front(std::move(task));
}

bool TaskQueue::Empty() const {
    std::lock_guard<std::mutex> lk(mtx);
    return task_deque.empty();
}

bool TaskQueue::Try_pop(Task& task) {
    std::lock_guard<std::mutex> lk(mtx);
    if (task_deque.empty()) {
        return false;
    } else {
        task = std::move(task_deque.front());
        task_deque.pop_front();
        return true;
    }
}

bool TaskQueue::Try_steal_task(Task& task) {
    std::lock_guard<std::mutex> lk(mtx);
    if (task_deque.empty()) {
        return false;
    } else {
        task = std::move(task_deque.back());
        task_deque.back();
        return true;
    }
}