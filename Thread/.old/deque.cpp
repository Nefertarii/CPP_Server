#include "Head/deque.h"

using namespace Wasi::Base;

template <typename T>
typename std::deque<T>::iterator SafeDeque<T>::Find(const T& value) {
    return std::find(data.Begin(), data.End(), value);
}

template <typename T>
SafeDeque<T>::SafeDeque(const SafeDeque& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
void SafeDeque<T>::Push_back(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(value);
}

template <typename T>
void SafeDeque<T>::Push_front(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_front(value);
}

template <typename T>
void SafeDeque<T>::Pop_back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_back();
}

template <typename T>
void SafeDeque<T>::Pop_front() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_front();
}

template <typename T>
void SafeDeque<T>::Insert(const T& posi_value, const T& insert_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = Find(posi_value);
    if (ret != data.end()) {
        data.insert(ret, insert_value);
    }
}

template <typename T>
void SafeDeque<T>::Earse(const T& posi_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = Find(posi_value);
    if (ret != data.end()) {
        data.erase(ret);
    }
}

template <typename T>
void SafeDeque<T>::Clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
void SafeDeque<T>::Swap(SafeDeque<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
void SafeDeque<T>::Lock() { mtx.lock(); }

template <typename T>
void SafeDeque<T>::Unlock() { mtx.unlock(); }

template <typename T>
bool SafeDeque<T>::Empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
uint SafeDeque<T>::Size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
typename std::deque<T>::iterator SafeDeque<T>::Begin() { return data.begin(); }

template <typename T>
typename std::deque<T>::iterator SafeDeque<T>::End() { return data.end(); }

template <typename T>
T SafeDeque<T>::Front() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.front();
}

template <typename T>
T SafeDeque<T>::Back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.back();
}