#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <mutex>
#include <algorithm>
#include <list>

namespace Wasi {
    namespace Thread {
        template <typename T>
        class Safe_List {
        private:
            /*struct List_Node {
                std::unique_ptr<T> data;
                std::shared_ptr<List_Node> next;
                std::shared_ptr<List_Node> prev;
                List_Node() {}
                List_Node(T const& value) :data(make_shared(value)) {}
            };
            List_Node head, tail;*/
            std::list<T> data;
            std::mutex mtx;
            typename std::list<T>::iterator find(const T& value);
        public:
            Safe_List() = default;
            Safe_List(const Safe_List& other);
            Safe_List operator=(const Safe_List&) = delete;
            bool find_first(T value);
            void push_back(const T& value);
            void push_front(const T& value);
            void pop_back();
            void pop_front();
            void insert(const T& posi_value, const T& insert_value);
            void earse(const T& posi_value);
            bool empty();
            void clear();
            uint size();
            void swap(Wasi::Thread::Safe_List<T>& rhs);
            typename std::list<T>::iterator begin();
            typename std::list<T>::iterator end();
            void lock();
            void unlock();
        };
    }
}

template <typename T>
typename std::list<T>::iterator Wasi::Thread::Safe_List<T>::find(const T& value) {
    return std::find(data.begin(), data.end(), value);
}

template <typename T>
Wasi::Thread::Safe_List<T>::Safe_List(const Safe_List& other) {
    std::lock_guard<std::mutex> lk(other.mtx);
    data = other.data;
}

template <typename T>
bool Wasi::Thread::Safe_List<T>::find_first(T value) {
    auto ret = find(value);
    if (ret == data.end()) { return false; }
    return true;
}

template <typename T>
void Wasi::Thread::Safe_List<T>::push_back(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(value);
}

template <typename T>
void Wasi::Thread::Safe_List<T>::push_front(const T& value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_front(value);
}

template <typename T>
void Wasi::Thread::Safe_List<T>::pop_back() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_back();
}

template <typename T>
void Wasi::Thread::Safe_List<T>::pop_front() {
    std::lock_guard<std::mutex> lk(mtx);
    data.pop_front();
}

template <typename T>
void Wasi::Thread::Safe_List<T>::insert(const T& posi_value, const T& insert_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = find(posi_value);
    if (ret != data.end()) {
        data.insert(ret, insert_value);
    }
}

template <typename T>
void Wasi::Thread::Safe_List<T>::earse(const T& posi_value) {
    std::lock_guard<std::mutex> lk(mtx);
    auto ret = find(posi_value);
    if (ret != data.end()) {
        data.erase(ret);
    }
}

template <typename T>
bool Wasi::Thread::Safe_List<T>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename T>
void Wasi::Thread::Safe_List<T>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename T>
uint Wasi::Thread::Safe_List<T>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename T>
void Wasi::Thread::Safe_List<T>::swap(Wasi::Thread::Safe_List<T>& rhs) {
    std::lock_guard<std::mutex> lk_lhs(this->mtx);
    std::lock_guard<std::mutex> lk_rhs(rhs.mtx);
    std::swap(this->data, rhs.data);
}

template <typename T>
typename std::list<T>::iterator Wasi::Thread::Safe_List<T>::begin() { return data.begin(); }

template <typename T>
typename std::list<T>::iterator Wasi::Thread::Safe_List<T>::end() { return data.end(); }

template <typename T>
void Wasi::Thread::Safe_List<T>::lock() { mtx.lock(); }

template <typename T>
void Wasi::Thread::Safe_List<T>::unlock() { mtx.unlock(); }


#endif