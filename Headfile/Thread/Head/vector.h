#include "concurrency.h"

#ifndef THREAD_VECTOR_H_
#define THREAD_VECTOR_H_

namespace thread {
    template <typename T>
    class Vector {
    private:
        /*
        */
        std::mutex mtx;
        void lock();
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