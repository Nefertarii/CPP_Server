#ifndef THREAD_SAFE_VECTOR_H_
#define THREAD_SAFE_VECTOR_H_

#include <mutex>
#include <vector>

namespace Thread {
    template <typename T>
    class Safe_Vector {
    private:
        std::vector<T> data;
        std::mutex mtx;
    public:
        Safe_Vector() = default;
        Safe_Vector(const Safe_Vector& other);
        Safe_Vector operator=(const Safe_Vector&) = delete;
        T get(uint posi);
        void get(uint posi, T& value);
        void insert(uint posi, T value);
        void push_back(T value);
        bool empty();
        void clear();
        T earse(uint posi);
        uint size();
        void resize(uint resize);
        void swap(Thread::Safe_Vector<T>& rhs);
        typename std::vector<T>::iterator begin();
        typename std::vector<T>::iterator end();
        void lock();
        void unlock();
    };
};


template <typename T>
Thread::Safe_Vector<T>::Safe_Vector(const Safe_Vector& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
T Thread::Safe_Vector<T>::get(uint posi) {
    std::lock_guard<std::mutex> lk(mtx);
    return data[posi];
}

template <typename T>
void Thread::Safe_Vector<T>::get(uint posi, T& var) {
    std::lock_guard<std::mutex> lk(mtx);
    var = data[posi];
}

template <typename T>
void Thread::Safe_Vector<T>::insert(uint posi, T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data[posi] = std::move(value);
}

template <typename T>
void Thread::Safe_Vector<T>::push_back(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(std::move(value));
}

template <typename T>
bool Thread::Safe_Vector<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
void Thread::Safe_Vector<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
T Thread::Safe_Vector<T>::earse(uint posi) {
    std::lock_guard<std::mutex> lk(mtx);
    T value = data.erase(data[posi]);
    return value;
}

template <typename T>
uint Thread::Safe_Vector<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void Thread::Safe_Vector<T>::resize(uint new_size) {
    std::lock_guard<std::mutex> lk(mtx);
    data.resize(new_size);
}

template <typename T>
void Thread::Safe_Vector<T>::swap(Thread::Safe_Vector<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
typename std::vector<T>::iterator Thread::Safe_Vector<T>::begin() { return data.begin(); }

template <typename T>
typename std::vector<T>::iterator Thread::Safe_Vector<T>::end() { return data.end(); }

template <typename T>
void Thread::Safe_Vector<T>::lock() { mtx.lock(); }

template <typename T>
void Thread::Safe_Vector<T>::unlock() { mtx.unlock(); }


#endif