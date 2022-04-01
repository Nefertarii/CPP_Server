#ifndef BASE_SAFE_QUEUE_H_
#define BASE_SAFE_QUEUE_H_

#include "../../../Class/noncopyable.h"
#include <condition_variable>
#include <memory>
#include <mutex>

namespace Wasi {
namespace Base {

template <typename T>
class SafeQueue : Noncopyable {
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
    Node* Get_tail();
    std::unique_lock<std::mutex> Wait_data();
    std::unique_ptr<Node> Pop_head();

public:
    SafeQueue();
    bool Try_pop(T& value);
    std::shared_ptr<T> Try_pop();
    std::shared_ptr<T> Wait_pop();
    void Wait_pop(T& value);
    void Push(T value);
    bool Empty();
};

}
} // namespace Wasi::Base

#endif // !BASE_SAFE_QUEUE_H_