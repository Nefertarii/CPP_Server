#ifndef THREAD_SAFE_VECTOR_H_
#define THREAD_SAFE_VECTOR_H_

#include "concurrency.h"
#include <vector>

namespace thread {
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
        void swap(thread::Safe_Vector<T>& rhs);
    };
};

#endif