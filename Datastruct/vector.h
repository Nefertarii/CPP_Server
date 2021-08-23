#ifndef VECTOR_H_
#define VECTOR_H_

#include <algorithm>

template <typename T>
class Vector
{
private:
    int size_;
    int capacity_;
    T *objects_;
    static const int SPARE_CAPACITY = 16;

public:
    using iterator = T *;
    using const_iterator = const T *;
    explicit Vector(int initSize = 0) : size_(initSize), capacity_(initSize + SPARE_CAPACITY) { objects_ = new T[capacity_]; }
    Vector(const Vector &rhs);
    Vector(Vector &&rhs);
    Vector &operator=(const Vector &rhs);
    Vector &operator=(Vector &&rhs);
    T &operator[](int index) { return objects_[index]; }
    const T &operator[](int index) const { return objects_[index]; }
    void resize(int newSize);
    void reserve(int newCapacity);
    int size() const { return size_; }
    int capacity() const { return capacity_; }
    bool empty() const { return size() == 0; }
    void push_back(const T &val);
    void push_back(T &&val);
    void pop_back() { --size_; }
    const T &back() const { return objects_[size_ - 1]; }
    iterator begin() { return &objects_[0]; }
    const_iterator begin() const { return &objects_[0]; }
    iterator end() { return &objects_[size_]; }
    const_iterator end() const { return &objects_[size_]; }
    ~Vector() { delete[] objects_; }
};

template <typename T>
Vector<T>::Vector(const Vector<T> &rhs)
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
Vector<T>::Vector(Vector<T> &&rhs)
{
    size_ = rhs.size_;
    rhs.size_ = 0;

    capacity_ = rhs.capacity_;
    rhs.capacity_ = 0;

    objects_ = rhs.objects_;
    rhs.objects_ = nullptr;
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &rhs)
{
    Vector copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> &&rhs)
{
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(objects_, rhs.objects_);
    return *this;
}

template <typename T>
void Vector<T>::resize(int newSize)
{
    if (newSize > capacity_)
        reserve(newSize * 2);
    size_ = newSize;
}

template <typename T>
void Vector<T>::reserve(int newCapacity)
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
void Vector<T>::push_back(const T &val)
{
    if (size_ == capacity_)
        reserve(capacity_ * 2 + 1);
    objects_[size_++] = val;
}

template <typename T>
void Vector<T>::push_back(T &&val)
{
    if (size_ == capacity_)
        reserve(capacity_ * 2 + 1);
    objects_[size_++] = std::move(val);
}

#endif