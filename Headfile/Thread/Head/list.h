#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include "concurrency.h"
#include <list>

namespace thread {
    template <typename T>
    class Safe_List {
    private:
        struct List_Node {
            std::unique_ptr<T> data;
            std::shared_ptr<List_Node> next;
            std::shared_ptr<List_Node> prev;
            List_Node(T value) :data(make_shared(value)) {}
        };
        std::list<T> data;
        std::mutex mtx;
        typename std::list<T>::iterator find(T value);
    public:
        Safe_List() = default;
        Safe_List(const Safe_List& other);
        Safe_List operator=(const Safe_List&) = delete;
        bool find_first(T value);
        void push_back(const T& value);
        void push_front(const T& value);
        void pop_back();
        void pop_front();
        void insert(const T& posi_value ,const T& value);
        void earse(const T& posi_value ,const T& value);
        bool empty();
        void clear();
        uint size();
    };
};

#endif