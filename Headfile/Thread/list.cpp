#include "Head/list.h"

template <typename T>
typename std::list<T>::iterator thread::Safe_List<T>::find(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto temp = data.begin();
    while (temp != data.end()) {
        if (value == ++temp) {
            return temp;
        }
    }
    return data.end();
}

template <typename T>
thread::Safe_List<T>::Safe_List(const Safe_List& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
bool thread::Safe_List<T>::find_first(T value) {
    auto ret = find(value);
    if (ret == data.end()) { return false; }
    return true;
}

template <typename T>
void thread::Safe_List<T>::push_back(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(value);
}

template <typename T>
void thread::Safe_List<T>::push_front(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_front(value);
}

template <typename T>
void thread::Safe_List<T>::pop_back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_back();
}

template <typename T>
void thread::Safe_List<T>::pop_front() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_front();
}

template <typename T>
void thread::Safe_List<T>::insert(const T& posi_value,const  T& value) {
    auto ret = find(value);
    if (ret != data.end()) {
        std::lock_guard<std::mutex> lk(mtx);
        data.insert(ret, value);
    }
}

template <typename T>
void thread::Safe_List<T>::earse(const T& posi_value,const  T& value) {
    auto ret = find(value);
    if (ret != data.end()) {
        std::lock_guard<std::mutex> lk(mtx);
        data.erase(ret, value);
    }
}

template <typename T>
bool thread::Safe_List<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
void thread::Safe_List<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
uint thread::Safe_List<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}