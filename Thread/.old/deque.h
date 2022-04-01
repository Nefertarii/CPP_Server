#ifndef BASE_SAFE_DEQUE_H_
#define BASE_SAFE_DEQUE_H_

#include <deque>
#include <mutex>

namespace Wasi {
namespace Base {

template <typename T>
class SafeDeque {
private:
    std::deque<T> data;
    mutable std::mutex mtx;
    typename std::deque<T>::iterator Find(const T& value);

public:
    SafeDeque() = default;
    SafeDeque(const SafeDeque& other);
    SafeDeque operator=(const SafeDeque&) = delete;
    void Push_back(T value);
    void Push_front(T value);
    void Pop_back();
    void Pop_front();
    void Insert(const T& posi_value, const T& value);
    void Earse(const T& posi_value);
    void Clear();
    void Swap(SafeDeque<T>& rhs);
    void Lock();
    void Unlock();
    bool Empty();
    uint Size();
    typename std::deque<T>::iterator Begin();
    typename std::deque<T>::iterator End();
    T Front();
    T Back();
};

}
} // namespace Wasi::Base

#endif // !BASE_SAFE_DEQUE_H_