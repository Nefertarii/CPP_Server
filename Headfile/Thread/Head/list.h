#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include "concurrency.h"
#include <list>

namespace thread {
    template <typename T>
    class Safe_List {
    private:
        std::list<T> data;
        std::mutex mtx;
    public:
        Safe_List() = default;
        Safe_List(const Safe_List& other);
        Safe_List operator=(const Safe_List&) = delete;
        T get(uint posi);
        void get(uint posi, T& value);
        void insert(uint posi, T value);
        void push_back(T value);
        bool empty();
        void clear();
        uint size();
    };
};