#include "Head/queue.h"

using namespace Wasi::Base;

template <typename T>
typename SafeQueue<T>::Node* SafeQueue<T>::Get_tail() {
    std::lock_guard<std::mutex> lk(tail_mtx);
    return tail;
}

template <typename T>
std::unique_lock<std::mutex> SafeQueue<T>::Wait_data() {
    std::unique_lock<std::mutex> head_lk(head_mtx);
    cond.wait(head_lk, [&] { return head.get() != Get_tail(); });
    return std::move(head_lk);
}

template <typename T>
std::unique_ptr<typename SafeQueue<T>::Node> SafeQueue<T>::Pop_head() {
    std::unique_ptr<Node> old_head = std::move(head);
    head                           = std::move(old_head->next);
    return old_head;
}

template <typename T>
SafeQueue<T>::SafeQueue() :
    head(std::make_unique<Node>()),
    tail(head.get()) {}

template <typename T>
bool SafeQueue<T>::Try_pop(T& value) {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    if (head.get() == Get_tail()) { return false; }
    value                                = std::move(*head->data);
    std::unique_ptr<Node> const old_head = Pop_head();
    return true;
}

template <typename T>
std::shared_ptr<T> SafeQueue<T>::Try_pop() {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    if (head.get() == Get_tail()) { return std::shared_ptr<Node>(); }
    std::unique_ptr<Node> old_head = Pop_head();
    return old_head;
}

template <typename T>
std::shared_ptr<T> SafeQueue<T>::Wait_pop() {
    std::unique_lock<std::mutex> head_lk(Wait_data());
    std::unique_ptr<Node> const old_head = Pop_head();
    return old_head->data;
}

template <typename T>
void SafeQueue<T>::Wait_pop(T& value) {
    std::unique_lock<std::mutex> head_lk(Wait_data());
    value                                = std::move(*head->data);
    std::unique_ptr<Node> const old_head = Pop_head();
}

template <typename T>
void SafeQueue<T>::Push(T value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
    std::unique_ptr<Node> tmp_node(new Node);
    {
        std::lock_guard<std::mutex> tail_lk(tail_mtx);
        tail->data           = new_data;
        Node* const new_tail = tmp_node.get();
        tail->next           = std::move(tmp_node);
        tail                 = new_tail;
    }
    cond.notify_one();
}

template <typename T>
bool SafeQueue<T>::Empty() {
    std::lock_guard<std::mutex> head_lk(head_mtx);
    return (head.get() == Get_tail());
}