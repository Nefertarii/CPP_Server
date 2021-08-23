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

// Thread safe implementation of a Queue using a std::queue
template <typename T>
class SafeQueue {
private:
    std::queue<T> queue_; //利用模板函数构造队列
    std::mutex m_mutex; // 访问互斥信号量
public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}
    bool empty(); // 返回队列是否为空
    int size();
    void enqueue(T &t); // 队列添加元素
    bool dequeue(T &t); // 队列取出元素
};

bool SafeQueue::empty() { 
    std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
    return queue_.empty();
}
int SafeQueue::size() {
    std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
    return queue_.size();
}
void SafeQueue::enqueue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex);
    queue_.emplace(t);
}
bool SafeQueue::dequeue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex); // 队列加锁
    if (queue_.empty())
        return false;
    t = std::move(queue_.front()); // 取出队首元素，返回队首元素值，并进行右值引用
    queue_.pop(); // 弹出入队的第一个元素
    return true;
}


class Gthreadpool {
private:
    class ThreadWorker { // 内置线程工作类
    private:
        int id_; // 工作id
        Gthreadpool *pool_; // 所属线程池
    public:
        // 构造函数
        ThreadWorker(Gthreadpool *pool, const int id) : pool_(pool), id_(id){}
        // 重载()操作
        void operator()() {
            std::function<void()> func; // 定义基础函数类func
            bool dequeued; // 是否正在取出队列中元素
            while (!pool_->shutdown_) {
                {
                    // 为线程环境加锁，互访问工作线程的休眠和唤醒
                    std::unique_lock<std::mutex> lock(pool_->conditional_mutex_);
                    // 如果任务队列为空，阻塞当前线程
                    if (pool_->queue_.empty()) {
                        pool_->conditional_lock_.wait(lock); // 等待条件变量通知，开启线程
                    }
                    // 取出任务队列中的元素
                    dequeued = pool_->queue_.dequeue(func);
                }
                // 如果成功取出，执行工作函数
                if (dequeued)
                    func();
            }
        }
    };
    bool shutdown_;                          // 线程池是否关闭;
    SafeQueue<std::function<void()>> queue_; // 执行函数安全队列，即任务队列
    std::vector<std::thread> threads_;       // 工作线程队列
    std::mutex conditional_mutex_;           // 线程休眠锁互斥变量
    std::condition_variable conditional_lock_; // 线程环境锁，可以让线程处于休眠或者唤醒状态

public:
    Gthreadpool(const int n_threads = 1) // 线程池构造函数
        : threads_(std::vector<std::thread>(n_threads)), shutdown_(false){}
    Gthreadpool(const Gthreadpool &) = delete;
    Gthreadpool(Gthreadpool &&) = delete;
    Gthreadpool &operator=(const Gthreadpool &) = delete;
    Gthreadpool &operator=(Gthreadpool &&) = delete;
    void init() { // Inits thread pool
        for (int i = 0; i < threads_.size(); ++i) {
            threads_.at(i) = std::thread(ThreadWorker(this, i)); // 分配工作线程
        }
    }
    // Waits until threads finish their current task and shutdowns the pool
    void shutdown() {
        shutdown_ = true;
        conditional_lock_.notify_all(); // 通知，唤醒所有工作线程
        for (int i = 0; i < threads_.size(); ++i) {
            if (threads_.at(i).joinable()) { // 判断线程是否在等待
                threads_.at(i).join(); // 将线程加入到等待队列
            }
        }
    }
    // Submit a function to be executed asynchronously by the pool
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
        // Create a function with bounded parameter ready to execute
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误
        // Encapsulate it into a shared pointer in order to be able to copy construct
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        // Warp packaged task into void function
        std::function<void()> warpper_func = [task_ptr]() {
            (*task_ptr)();
        };
        // 队列通用安全封包函数，并压入安全队列
        queue_.enqueue(warpper_func);
        // 唤醒一个等待中的线程
        conditional_lock_.notify_one();
        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
};

#endif