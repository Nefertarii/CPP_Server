#ifndef THREAD_SAFE_DEQUE_H_
#define THREAD_SAFE_DEQUE_H_

#include <mutex>
#include <deque>

namespace Thread {
    template <typename T>
    class Safe_Deque {
    private:
        std::deque<T> data;
        std::mutex mtx;
        typename std::deque<T>::iterator find(const T& value);
    public:
        Safe_Deque() = default;
        Safe_Deque(const Safe_Deque& other);
        Safe_Deque operator=(const Safe_Deque&) = delete;
        void push_back(T value);
        void push_front(T value);
        void pop_back();
        void pop_front();
        T front();
        T back();
        void insert(const T& posi_value, const T& value);
        void earse(const T& posi_value);
        bool empty();
        void clear();
        uint size();
        void swap(Thread::Safe_Deque<T>& rhs);
        typename std::deque<T>::iterator begin();
        typename std::deque<T>::iterator end();
        void lock();
        void unlock();
    };
}

template <typename T>
typename std::deque<T>::iterator Thread::Safe_Deque<T>::find(const T& value) {
    return std::find(data.begin(), data.end(), value);
}

template <typename T>
Thread::Safe_Deque<T>::Safe_Deque(const Safe_Deque& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
void Thread::Safe_Deque<T>::push_back(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(value);
}

template <typename T>
void Thread::Safe_Deque<T>::push_front(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_front(value);
}

template <typename T>
T Thread::Safe_Deque<T>::front() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.front();
}

template <typename T>
T Thread::Safe_Deque<T>::back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.back();
}

template <typename T>
void Thread::Safe_Deque<T>::pop_back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_back();
}

template <typename T>
void Thread::Safe_Deque<T>::pop_front() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_front();
}

template <typename T>
void Thread::Safe_Deque<T>::insert(const T& posi_value, const T& insert_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = find(posi_value);
    if (ret != data.end()) {
        data.insert(ret, insert_value);
    }
}

template <typename T>
void Thread::Safe_Deque<T>::earse(const T& posi_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = find(posi_value);
    if (ret != data.end()) {
        data.erase(ret);
    }
}

template <typename T>
bool Thread::Safe_Deque<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
void Thread::Safe_Deque<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
uint Thread::Safe_Deque<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void Thread::Safe_Deque<T>::swap(Thread::Safe_Deque<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
typename std::deque<T>::iterator Thread::Safe_Deque<T>::begin() { return data.begin(); }

template <typename T>
typename std::deque<T>::iterator Thread::Safe_Deque<T>::end() { return data.end(); }

template <typename T>
void Thread::Safe_Deque<T>::lock() { mtx.lock(); }

template <typename T>
void Thread::Safe_Deque<T>::unlock() { mtx.unlock(); }


#endif