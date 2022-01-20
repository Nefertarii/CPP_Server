#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include "concurrency.h"
#include <queue>

namespace Thread {
    template <typename T>
    class Safe_Queue {
    private:
        std::queue<T> data;
        std::mutex mtx;
    public:
        Safe_Queue() = default;
        Safe_Queue(const Safe_Queue& other);
        Safe_Queue operator=(const Safe_Queue&) = delete;
        void push(const T& value);
        void pop();
        bool empty();
        uint size();
        void swap(Thread::Safe_Queue<T>& rhs);
        void lock();
        void unlock();
    };
}

template <typename T>
Thread::Safe_Queue<T>::Safe_Queue(const Safe_Queue& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
void Thread::Safe_Queue<T>::push(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push(value);
}

template <typename T>
void Thread::Safe_Queue<T>::pop() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop();
}

template <typename T>
bool Thread::Safe_Queue<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
uint Thread::Safe_Queue<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void Thread::Safe_Queue<T>::swap(Thread::Safe_Queue<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
void Thread::Safe_Queue<T>::lock() { mtx.lock(); }

template <typename T>
void Thread::Safe_Queue<T>::unlock() { mtx.unlock(); }


#endif