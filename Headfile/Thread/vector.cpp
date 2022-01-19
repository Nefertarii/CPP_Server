#include "Head/vector.h"

template <typename T>
thread::Safe_Vector<T>::Safe_Vector(const Safe_Vector& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
T thread::Safe_Vector<T>::get(uint posi) {
    std::lock_guard<std::mutex> lk(mtx);
    return data[posi];
}

template <typename T>
void thread::Safe_Vector<T>::get(uint posi, T& var) {
    std::lock_guard<std::mutex> lk(mtx);
    var = data[posi];
}

template <typename T>
void thread::Safe_Vector<T>::insert(uint posi, T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data[posi] = std::move(value);
}

template <typename T>
void thread::Safe_Vector<T>::push_back(T value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(std::move(value));
}

template <typename T>
bool thread::Safe_Vector<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
void thread::Safe_Vector<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
T thread::Safe_Vector<T>::earse(uint posi) {
    std::lock_guard<std::mutex> lk(mtx);
    T value = data.erase(data[posi]);
    return value;
}

template <typename T>
uint thread::Safe_Vector<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void thread::Safe_Vector<T>::resize(uint new_size) {
    std::lock_guard<std::mutex> lk(mtx);
    data.resize(new_size);
}

template <typename T>
void thread::Safe_Vector<T>::swap(thread::Safe_Vector<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}
