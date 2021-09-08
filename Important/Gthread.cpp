#include "Gthread.h"


ULL Pthreadid() {
    std::ostringstream buf;
    buf << std::this_thread::get_id();
    std::string stid = buf.str();
    ULL tid = std::stoull(stid);
    return tid;
}

template <typename T>
bool Safequeue<T>::empty() { 
    std::unique_lock<std::mutex> lock(m_mutex); 
    return queue_.empty();
}

template <typename T>
int Safequeue<T>::size() {
    std::unique_lock<std::mutex> lock(m_mutex); 
    return queue_.size();
}

template <typename T>
void Safequeue<T>::enqueue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex);
    queue_.emplace(t);
}

template <typename T>
bool Safequeue<T>::dequeue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex); 
    if (queue_.empty())
        return false;
    t = std::move(queue_.front()); 
    queue_.pop(); 
    return true;
}


void Gthreadpool::ThreadWorker::operator()() {
    std::function<void()> func;
    bool dequeued;
    while (!pool_->shutdown_) {
        { //RAII
            std::unique_lock<std::mutex> lock(pool_->conditional_mutex_);
            if (pool_->queue_.empty()) {
                pool_->conditional_lock_.wait(lock);
            }
            dequeued = pool_->queue_.dequeue(func);
        }
        if (dequeued)
            func();
    }
}


void Gthreadpool::init() {
    shutdown_ = false;
    threads_ = std::vector<std::thread>(threadnum_);
    for (uint i = 0; i < threads_.size(); ++i) {
        threads_.at(i) = std::thread(ThreadWorker(this, i));
    }
}

void Gthreadpool::init(const size_t threadnum) {
    threadnum_ = threadnum;
    init();
}

void Gthreadpool::shutdown() {
    shutdown_ = true;
    conditional_lock_.notify_all(); 
    for (uint i = 0; i < threads_.size(); ++i) {
        if (threads_.at(i).joinable()) { 
            threads_.at(i).join(); 
        }
    }
}

// Submit a function to be executed asynchronously by the pool
template <class F, class... Args>
auto Gthreadpool::submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    // Create a function with bounded parameter ready to execute
    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // ���Ӻ����Ͳ������壬���⺯�����ͣ���������ֵ����
    // Encapsulate it into a shared pointer in order to be able to copy construct
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
    // Warp packaged task into void function
    std::function<void()> warpper_func = [task_ptr]() {
        (*task_ptr)();
    };
    queue_.enqueue(warpper_func);
    conditional_lock_.notify_one();
    return task_ptr->get_future();
}

