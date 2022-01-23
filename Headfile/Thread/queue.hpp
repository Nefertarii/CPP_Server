#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <memory>
#include <mutex>
#include <condition_variable>



namespace Thread {
    template <typename T>
    class Safe_Queue {
    private:
        struct Node {
            std::shared_ptr<T> data;
            std::unique_ptr<Node> next;
        };
        std::unique_ptr<Node> head;
        Node* tail;
        std::mutex head_mtx;
        std::mutex tail_mtx;
        std::condition_variable cond;
        Node* get_tail();
        std::unique_lock<std::mutex> wait_data();
        std::unique_ptr<Node> pop_head();
    public:
        Safe_Queue() :head(new Node), tail(head.get()) {}
        Safe_Queue(const Safe_Queue& other) = delete;
        Safe_Queue operator=(const Safe_Queue&) = delete;
        bool try_pop(T& value);
        std::shared_ptr<T> try_pop();
        std::shared_ptr<T> wait_pop();
        void wait_pop(T& value);
        void push(T value);
        bool empty();
    };
}

template <typename T>
typename Thread::Safe_Queue<T>::Node* Thread::Safe_Queue<T>::get_tail() {
    std::lock_guard<std::mutex> lk(tail_mtx);
    return tail;
}

template <typename T>
std::unique_lock<std::mutex> Thread::Safe_Queue<T>::wait_data() {
    std::unique_lock<std::mutex> head_lk(head_mtx);
    cond.wait(head_lk, [&] {return head.get() != get_tail();});
    return std::move(head_lk);
}

template <typename T>
std::unique_ptr<typename Thread::Safe_Queue<T>::Node> Thread::Safe_Queue<T>::pop_head() {
    std::unique_ptr<Node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
}

template <typename T>
bool Thread::Safe_Queue<T>::try_pop(T& value) {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    if (head.get() == get_tail()) { return false; }
    value = std::move(*head->data);
    std::unique_ptr<Node> const old_head = pop_head();
    return true;
}

template <typename T>
std::shared_ptr<T> Thread::Safe_Queue<T>::try_pop() {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    if (head.get() == get_tail()) { return std::shared_ptr<Node>(); }
    std::unique_ptr<Node> old_head = pop_head();
    return old_head;
}

template <typename T>
std::shared_ptr<T> Thread::Safe_Queue<T>::wait_pop() {
    std::unique_lock<std::mutex> head_lk(wait_data());
    std::unique_ptr<Node> const old_head = pop_head();
    return old_head->data;
}

template <typename T>
void Thread::Safe_Queue<T>::wait_pop(T& value) {
    std::unique_lock<std::mutex> head_lk(wait_data());
    value = std::move(*head->data);
    std::unique_ptr<Node> const old_head = pop_head();
}

template <typename T>
void Thread::Safe_Queue<T>::push(T value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
    std::unique_ptr<Node> tmp_node(new Node);
    {
        std::lock_guard<std::mutex> tail_lk(tail_mtx);
        tail->data = new_data;
        Node* const new_tail = tmp_node.get();
        tail->next = std::move(tmp_node);
        tail = new_tail;
    }
    cond.notify_one();
}

template <typename T>
bool Thread::Safe_Queue<T>::empty() {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    return (head.get() == get_tail());
}




#endif