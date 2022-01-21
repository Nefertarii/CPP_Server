#include "deque.hpp"
#include "queue.hpp"
#include "vector.hpp"
#include <functional>
#include <exception>
#include <future>

namespace Thread {
    class Thread_Pool {
    private:
        struct Impl_base {
            virtual void call() = 0;
            virtual ~Impl_base() {}
        };
        template <typename Func>
        struct Impl_type : Impl_base {
            Func func;
            Impl_type(Func&& func_) :func(std::move(func_)) {}
            void call() { func(); }
        };
        class Function_wrap {
        private:
            std::unique_ptr<Impl_base> impl;
        public:
            Function_wrap() = default;
            template <typename Func>
            Function_wrap(Func&& func) :impl(new Impl_type<Func>(std::move(func))) {}
            void operator()() { impl->call(); }
            Function_wrap(Function_wrap&& other) :impl(std::move(other.impl)) {}
            Function_wrap& operator=(Function_wrap&& other);
            Function_wrap(const Function_wrap&) = delete;
            Function_wrap(Function_wrap&) = delete;
            Function_wrap& operator=(const Function_wrap&) = delete;
        };
        using Task = Function_wrap;
        class Task_deque {
        private:
            std::deque<Task> deque;
            mutable std::mutex mtx;
        public:
            Task_deque() {}
            Task_deque(const Task_deque& other) = delete;
            Task_deque& operator=(const Task_deque& other) = delete;
            void push();
            bool empty();
            bool try_pop();
            bool try_steal_task();
        };
        class Join_threads {
        private:
            std::vector<std::thread>& threads;
        public:
            explicit Join_threads(std::vector<std::thread>& threads_) :
                threads(threads_) {}
            ~Join_threads();
        };
        std::atomic_bool done;
        Thread::Safe_Queue<Task> pool_work_queue;
        std::vector<std::unique_ptr<Task_deque>> queues;
        std::vector<std::thread> threads;
        Join_threads joiner;
        static thread_local Task_deque* local_work_deque;
        static thread_local int index;
        void worker_thread(int index_);
        bool pop_task_from_local_queue(Task& task);
        bool pop_task_from_pool_queue(Task& task);
        bool pop_task_from_other_thread_queue(Task& task);
    public:
        Thread_Pool();
        template <typename Func>
        std::future<typename std::result_of<Func()>::type> submit(Func func);
        void run_pending_task();
        ~Thread_Pool();
    };
};

Function_wrap Thread::Thread_Pool::Function_wrap::operator=(Function_wrap&& other) {
    impl = std::move(other.impl);
    return *this;
}

void Thread::Thread_Pool::Task_deque::push(data_type data) {
    std::lock_guard<std::mutex> lk(mtx);
    deque.push_front(std::move(data));
}

bool Thread::Thread_Pool::Task_deque::empty() const {
    std::lock_guard<std::mutex> lk(mtx);
    return deque.empty();
}

bool Thread::Thread_Pool::Task_deque::try_pop(data_type& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) { return false; }
    else {
        res = std::move(deque.front());
        deque.pop_front();
        return true;
    }
}

bool Thread::Thread_Pool::Task_deque::try_steal_task(data_type& res) {
    std::lock_guard<std::mutex> lk(mtx);
    if (deque.empty()) { return false; }
    else {
        res = std::move(deque.back());
        deque.pop_back();
        return true;
    }
}

Thread::Thread_Pool::Join_threads::~Join_threads() {
    for (uint i = 0;i < threads.size();i++) {
        if (threads[i].joinable()) { threads[i].join(); }
    }
}



