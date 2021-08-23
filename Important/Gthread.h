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
    std::queue<T> queue_; //����ģ�庯���������
    std::mutex m_mutex; // ���ʻ����ź���
public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}
    bool empty(); // ���ض����Ƿ�Ϊ��
    int size();
    void enqueue(T &t); // �������Ԫ��
    bool dequeue(T &t); // ����ȡ��Ԫ��
};

bool SafeQueue::empty() { 
    std::unique_lock<std::mutex> lock(m_mutex); // �����źű�����������ֹm_queue���ı�
    return queue_.empty();
}
int SafeQueue::size() {
    std::unique_lock<std::mutex> lock(m_mutex); // �����źű�����������ֹm_queue���ı�
    return queue_.size();
}
void SafeQueue::enqueue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex);
    queue_.emplace(t);
}
bool SafeQueue::dequeue(T &t) {
    std::unique_lock<std::mutex> lock(m_mutex); // ���м���
    if (queue_.empty())
        return false;
    t = std::move(queue_.front()); // ȡ������Ԫ�أ����ض���Ԫ��ֵ����������ֵ����
    queue_.pop(); // ������ӵĵ�һ��Ԫ��
    return true;
}


class Gthreadpool {
private:
    class ThreadWorker { // �����̹߳�����
    private:
        int id_; // ����id
        Gthreadpool *pool_; // �����̳߳�
    public:
        // ���캯��
        ThreadWorker(Gthreadpool *pool, const int id) : pool_(pool), id_(id){}
        // ����()����
        void operator()() {
            std::function<void()> func; // �������������func
            bool dequeued; // �Ƿ�����ȡ��������Ԫ��
            while (!pool_->shutdown_) {
                {
                    // Ϊ�̻߳��������������ʹ����̵߳����ߺͻ���
                    std::unique_lock<std::mutex> lock(pool_->conditional_mutex_);
                    // ����������Ϊ�գ�������ǰ�߳�
                    if (pool_->queue_.empty()) {
                        pool_->conditional_lock_.wait(lock); // �ȴ���������֪ͨ�������߳�
                    }
                    // ȡ����������е�Ԫ��
                    dequeued = pool_->queue_.dequeue(func);
                }
                // ����ɹ�ȡ����ִ�й�������
                if (dequeued)
                    func();
            }
        }
    };
    bool shutdown_;                          // �̳߳��Ƿ�ر�;
    SafeQueue<std::function<void()>> queue_; // ִ�к�����ȫ���У����������
    std::vector<std::thread> threads_;       // �����̶߳���
    std::mutex conditional_mutex_;           // �߳��������������
    std::condition_variable conditional_lock_; // �̻߳��������������̴߳������߻��߻���״̬

public:
    Gthreadpool(const int n_threads = 1) // �̳߳ع��캯��
        : threads_(std::vector<std::thread>(n_threads)), shutdown_(false){}
    Gthreadpool(const Gthreadpool &) = delete;
    Gthreadpool(Gthreadpool &&) = delete;
    Gthreadpool &operator=(const Gthreadpool &) = delete;
    Gthreadpool &operator=(Gthreadpool &&) = delete;
    void init() { // Inits thread pool
        for (int i = 0; i < threads_.size(); ++i) {
            threads_.at(i) = std::thread(ThreadWorker(this, i)); // ���乤���߳�
        }
    }
    // Waits until threads finish their current task and shutdowns the pool
    void shutdown() {
        shutdown_ = true;
        conditional_lock_.notify_all(); // ֪ͨ���������й����߳�
        for (int i = 0; i < threads_.size(); ++i) {
            if (threads_.at(i).joinable()) { // �ж��߳��Ƿ��ڵȴ�
                threads_.at(i).join(); // ���̼߳��뵽�ȴ�����
            }
        }
    }
    // Submit a function to be executed asynchronously by the pool
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
        // Create a function with bounded parameter ready to execute
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // ���Ӻ����Ͳ������壬���⺯�����ͣ���������ֵ����
        // Encapsulate it into a shared pointer in order to be able to copy construct
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        // Warp packaged task into void function
        std::function<void()> warpper_func = [task_ptr]() {
            (*task_ptr)();
        };
        // ����ͨ�ð�ȫ�����������ѹ�밲ȫ����
        queue_.enqueue(warpper_func);
        // ����һ���ȴ��е��߳�
        conditional_lock_.notify_one();
        // ������ǰע�������ָ��
        return task_ptr->get_future();
    }
};

#endif