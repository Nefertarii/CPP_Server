#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace Wasi {
namespace Base {

template <typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond;

public:
    SafeQueue() :
        queue(), mtx(), cond() {}
    SafeQueue(const SafeQueue& other) = delete;
    SafeQueue operator=(const SafeQueue&) = delete;
    void Push(T value) {
        std::lock_guard<std::mutex> lk(mtx);
        queue.push(value);
        cond.notify_one();
    }
    bool Empty() {
        std::lock_guard<std::mutex> lk(mtx);
        return queue.empty();
    }
    bool Try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mtx);
        if (queue.empty()) {
            return false;
        } else {
            value = queue.front();
            queue.pop();
            return true;
        }
    }
    T Try_pop() {
        std::lock_guard<std::mutex> lk(mtx);
        if (queue.empty()) {
            return T();
        } else {
            T value = queue.front();
            queue.pop();
            return value;
        }
    }
    T Wait_pop() {
        std::unique_lock<std::mutex> lk(mtx);
        cond.wait(lk);
        T value = queue.front();
        return value;
    }
};

}
} // namespace Wasi::Base

#endif