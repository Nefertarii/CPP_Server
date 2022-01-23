#ifndef THREAD_SAFE_STACK_H_
#define THREAD_SAFE_STACK_H_

#include <mutex>
#include <stack>

namespace Thread {
    template <typename T>
    class Safe_Stack {
    private:
        std::stack<T> data;
        std::mutex mtx;
    public:
        Safe_Stack() = default;
        Safe_Stack(const Safe_Stack& other);
        Safe_Stack operator=(const Safe_Stack&) = delete;
        void push(const T& value);
        void pop();
        bool empty();
        uint size();
        void swap(Thread::Safe_Stack<T>& rhs);
        void lock();
        void unlock();
    };
}

template <typename T>
Thread::Safe_Stack<T>::Safe_Stack(const Safe_Stack& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
void Thread::Safe_Stack<T>::push(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push(value);
}

template <typename T>
void Thread::Safe_Stack<T>::pop() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop();
}

template <typename T>
bool Thread::Safe_Stack<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
uint Thread::Safe_Stack<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void Thread::Safe_Stack<T>::swap(Thread::Safe_Stack<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
void Thread::Safe_Stack<T>::lock() { mtx.lock(); }

template <typename T>
void Thread::Safe_Stack<T>::unlock() { mtx.unlock(); }


#endif