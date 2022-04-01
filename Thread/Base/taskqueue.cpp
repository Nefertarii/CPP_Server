#include "Head/taskqueue.h"

using namespace Wasi::Base;

void TaskQueue::push(Task task) {
    std::lock_guard<std::mutex> lk(mtx);
    task_deque.push(std::move(task));
}

bool TaskQueue::empty() const {
    std::lock_guard<std::mutex> lk(mtx);
    return task_deque.empty();
}

bool TaskQueue::try_pop(Task& task) {
    std::lock_guard<std::mutex> lk(mtx);
    if (task_deque.empty()) {
        return false;
    } else {
        task = std::move(task_deque.front());
        task_deque.pop_front();
        return true;
    }
}

bool TaskQueue::try_steal_task(Task& task) {
    std::lock_guard<std::mutex> lk(mtx);
    if (task_deque.empty()) {
        return false;
    } else {
        task = std::move(task_deque.back());
        task_deque.back();
        return true;
    }
}