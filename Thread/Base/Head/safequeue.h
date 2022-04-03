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
    SafeQueue();
    SafeQueue(const SafeQueue& other) = delete;
    SafeQueue operator=(const SafeQueue&) = delete;
    void Notifi_all();
    void Notifi_one();
    void Push(T value);
    bool Empty();
    bool Try_pop(T& value);
    T Try_pop();
    T Wait_pop();
};
template <typename T>
SafeQueue<T>::SafeQueue() :
    queue(), mtx(), cond() {}

template <typename T>
void SafeQueue<T>::Notifi_all() { cond.notify_one(); }

template <typename T>
void SafeQueue<T>::Notifi_one() { cond.notify_all(); }

template <typename T>
void SafeQueue<T>::Push(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    queue.push(value);
    cond.notify_one();
}

template <typename T>
bool SafeQueue<T>::Empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return queue.empty();
}

template <typename T>
bool SafeQueue<T>::Try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    if (queue.empty()) {
        return false;
    } else {
        value = queue.front();
        queue.pop();
        return true;
    }
}

template <typename T>
T SafeQueue<T>::Try_pop() {
    std::lock_guard<std::mutex> lk(mtx);
    if (queue.empty()) {
        return T();
    } else {
        T value = queue.front();
        queue.pop();
        return value;
    }
}

template <typename T>
T SafeQueue<T>::Wait_pop() {
    std::unique_lock<std::mutex> lk(mtx);
    cond.wait(lk);
    T value = queue.front();
    return value;
}

}
} // namespace Wasi::Base

#endif