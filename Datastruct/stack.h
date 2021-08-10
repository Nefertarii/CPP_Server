#ifndef STACK_H_
#define STACK_H_

#include <algorithm>
//array based stack
template <typename T>
class Stack
{
private:
    int size_;
    int capacity_;
    T *objects_;
    static const int SPARE_CAPACITY = 16;
    void reserve(int newCapacity);

public:
    explicit Stack(int initSize = 0) : size_(initSize), capacity_(initSize + SPARE_CAPACITY) { objects_ = new T[capacity_]; }
    Stack(const Stack &rhs);
    Stack(Stack &&rhs);
    Stack &operator=(const Stack &rhs);
    Stack &operator=(Stack &&rhs);
    int size() const { return size_; }
    bool empty() const { return size() == 0; }
    T top() const { return objects_[size_ - 1]; }
    void push(const T &val);
    void push(T &&val);
    //void emplace();
    void pop() { --size_; }

    ~Stack() { delete[] objects_; }
};

template <typename T>
void Stack<T>::reserve(int newCapacity)
{
    if (newCapacity < size_)
        return;
    T *newArray = new T[newCapacity];
    for (int i = 0; i != capacity_; i++)
    {
        newArray[i] = std::move(objects_[i]);
    }
    capacity_ = newCapacity;
    std::swap(objects_, newArray);
    delete[] newArray;
}

template <typename T>
Stack<T>::Stack(const Stack<T> &rhs)
{
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
    objects_ = new T[capacity_];
    for (int i = 0; i < size_; i++)
    {
        objects_[i] = rhs.objects_[i];
    }
}

template <typename T>
Stack<T>::Stack(Stack<T> &&rhs)
{
    size_ = rhs.size_;
    rhs.size_ = 0;

    capacity_ = rhs.capacity_;
    rhs.capacity_ = 0;

    objects_ = rhs.objects_;
    rhs.objects_ = nullptr;
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &rhs)
{
    Stack copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template <typename T>
Stack<T> &Stack<T>::operator=(Stack<T> &&rhs)
{
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(objects_, rhs.objects_);
    return *this;
}

template <typename T>
void Stack<T>::push(const T &val)
{
    if (size_ == capacity_)
        reserve(capacity_ * 2 + 1);
    objects_[size_++] = val;
}

template <typename T>
void Stack<T>::push(T &&val)
{
    if (size_ == capacity_)
        reserve(capacity_ * 2 + 1);
    objects_[size_++] = std::move(val);
}

#endif