#include "concurrency.h"
#include <vector>

#ifndef THREAD_VECTOR_H_
#define THREAD_VECTOR_H_

namespace thread {
    template <typename T>
    class Vector {
    private:
        std::vector<T> data;
        std::mutex mtx;
    public:
        Vector() = default;
        Vector(const Vector& other);
        Vector operator=(const Vector&) = delete;
        T get(uint posi);
        void get(uint posi, T& value);
        void insert(uint posi, T value);
        void push_back(T value);
        bool empty();
        uint size();
        void clear();
    };
};

#endif

template <typename T>
thread::Vector<T>::Vector(const Vector& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
T thread::Vector<T>::get(uint posi) {
    std::lock_guard<std::mutex> lk(mtx);
    return data[posi];
}

template <typename T>
void thread::Vector<T>::get(uint posi, T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    value = data[posi];
}

template <typename T>
void thread::Vector<T>::insert(uint posi, T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data[posi] = std::move(value);
}

template <typename T>
void thread::Vector<T>::push_back(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(std::move(value));
}

template <typename T>
bool thread::Vector<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
uint thread::Vector<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void thread::Vector<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}