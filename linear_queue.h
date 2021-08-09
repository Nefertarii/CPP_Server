#ifndef LINEAR_QUEUE_H_
#define LINEAR_QUEUE_H_

#include "queue_T.h"

//Array-based queue
template <class Elem>
class linear_Queue : public queue_T<Elem>
{
private:
    int size;
    int front;
    int rear;
    Elem *listArray;

public:
    linear_Queue(int sz = DefaultListSize);
    ~linear_Queue() { delete[] listArray; }
    void clear() { front = rear; }
    bool enqueue(const Elem &);
    bool dequeue(Elem &it);
    bool frontValue(Elem &it) const;
    virtual int length() const { return ((rear + size) - front - 1) % size; }
};

template <class Elem>
linear_Queue<Elem>::linear_Queue(int sz)
{
    size = sz + 1;
    rear = 0;
    front = 1;
    listArray = new Elem[size];
}

template <class Elem>
bool linear_Queue<Elem>::enqueue(const Elem &it)
{
    if (((rear + 2) % size) == front)
        return false;
    rear = (rear + 1) % size;
    listArray[rear] = it;
    return true;
}

template <class Elem>
bool linear_Queue<Elem>::dequeue(Elem &it)
{
    if (length() == 0)
        return false;
    it = listArray[front];
    front = (front + 1) % size;
    return true;
}

template <class Elem>
bool linear_Queue<Elem>::frontValue(Elem &it) const
{
    if (length() == 0)
        return false;
    it = listArray[front];
    return true;
}

#endif