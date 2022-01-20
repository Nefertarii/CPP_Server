#ifndef THREAD_SAFE_MAP_H_
#define THREAD_SAFE_MAP_H_

#include "concurrency.h"
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
        typename std::map<Key, Value>::iterator begin();
        typename std::map<Key, Value>::iterator end();
        void swap(const Safe_Map& other);
        void lock();
        void unlock();
    };
};



#endif