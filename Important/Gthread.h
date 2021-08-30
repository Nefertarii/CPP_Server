#ifndef GTHREAD_H
#define GTHREAD_H

#include <mutex>
#include <iomanip>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <sstream>

using ULL = unsigned long long

ULL Pthreadid();


// Thread safe implementation of a Queue using a std::queue
template <typename T>
class Safequeue {
private:
    std::queue<T> queue_; 
    std::mutex m_mutex; 
public:
    Safequeue() {}
    Safequeue(Safequeue &&other) {}
    ~Safequeue() {}
    bool empty();
    int size();
    void enqueue(T &t);
    bool dequeue(T &t);
};

class Gthreadpool {
private:
    class ThreadWorker { 
    private:
        int id_; 
        Gthreadpool *pool_; 
    public:
        ThreadWorker(Gthreadpool *pool, const int id) : pool_(pool), id_(id){}
        void operator()();
    };
    bool shutdown_;                          
    Safequeue<std::function<void()>> queue_; 
    std::vector<std::thread> threads_;       
    std::mutex conditional_mutex_;           
    std::condition_variable conditional_lock_;

public:
    Gthreadpool(const int n_threads = 1) 
        : threads_(std::vector<std::thread>(n_threads)), shutdown_(false){}
    Gthreadpool(const Gthreadpool &) = delete;
    Gthreadpool(Gthreadpool &&) = delete;
    Gthreadpool &operator=(const Gthreadpool &) = delete;
    Gthreadpool &operator=(Gthreadpool &&) = delete;
    void init();
    void shutdown();
    // Submit a function to be executed asynchronously by the pool
    template <class F, class... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>;
};

#endif