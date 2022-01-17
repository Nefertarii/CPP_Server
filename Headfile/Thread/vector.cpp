#include "Head/vector.h"

template <typename T>
void thread::Vector<T>::Vector::lock() {
    std::lock_guard<std::mutex> lk(mtx);
    //if time out
    //...
}

template <typename T>
thread::Vector<T>::Vector(const Vector& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
T thread::Vector<T>::get(uint posi) {
    lock();
    return data[posi];
}

template <typename T>
void thread::Vector<T>::get(uint posi, T& value) {
    lock();
    value = data[posi];
}

template <typename T>
void thread::Vector<T>::insert(uint posi, T value) {
    lock();
    data[posi] = std::move(value);
}

template <typename T>
void thread::Vector<T>::push_back(T value) {
    lock();
    data.push_back(std::move(value));
}

template <typename T>
bool thread::Vector<T>::empty() {
    lock();
    return data.empty();
}

template <typename T>
uint thread::Vector<T>::size() {
    lock();
    return data.size();
}

template <typename T>
void thread::Vector<T>::clear() {
    lock();
    data.clear();
}