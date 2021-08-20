#ifndef GTHREAD_H_
#define GTHREAD_H_

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

using Tid = std::thread::id;
static const size_t MAXthread = 10;

class Gthreadpool {
private:
    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()>> tasks_;
    };
    std::shared_ptr<data> data_;

public:
    explicit Gthreadpool(size_t thread_count);
    Gthreadpool() = default;
    Gthreadpool(Gthreadpool &&) = default;
    template <class F>
    void execute(F &&task);
    ~Gthreadpool();
};


#endif