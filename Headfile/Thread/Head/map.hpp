#ifndef THREAD_SAFE_MAP_H_
#define THREAD_SAFE_MAP_H_

#include <mutex>
#include <map>

namespace Thread {
    template <typename Key, typename Value>
    class Safe_Map {
    private:
        std::map<Key, Value> data;
        std::mutex mtx;
    public:
        Safe_Map() = default;
        Safe_Map operator=(const Safe_Map&) = delete;
        uint size();
        bool empty();
        void clear();
        void insert(Key key, Value value);
        void earse(Key earse_key);
        void swap(const Safe_Map& other);
        typename std::map<Key, Value>::iterator begin();
        typename std::map<Key, Value>::iterator end();
        void lock();
        void unlock();
    };
};

template <typename Key, typename Value>
uint Thread::Safe_Map<Key, Value>::size() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.size();
}

template <typename Key, typename Value>
bool Thread::Safe_Map<Key, Value>::empty() {
    std::lock_guard<std::mutex> lk(mtx);
    return data.empty();
}

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::clear() {
    std::lock_guard<std::mutex> lk(mtx);
    data.clear();
}

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::insert(Key key, Value value) {
    std::lock_guard<std::mutex> lk(mtx);
    data.insert(key, value);
}

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::earse(Key earse_key) {
    std::lock_guard<std::mutex> lk(mtx);
    data.erase(earse_key);
}

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::swap(const Safe_Map& other) {
    std::lock_guard<std::mutex> lk(mtx);
    std::swap(data, other);
}

template <typename Key, typename Value>
typename std::map<Key, Value>::iterator Thread::Safe_Map<Key, Value>::begin() {
    return data.begin();
}

template <typename Key, typename Value>
typename std::map<Key, Value>::iterator Thread::Safe_Map<Key, Value>::end() {
    return data.end();
}

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::lock() { mtx.lock(); }

template <typename Key, typename Value>
void Thread::Safe_Map<Key, Value>::unlock() { mtx.unlock(); }



#endif