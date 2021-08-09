#ifndef QUEUE_H_
#define QUEUE_H_

#include "list_T.h"
#include "queue_T.h"

//Linked queue
template <class Elem>
class Queue : public queue_T<Elem>
{
private:
    Link<Elem> *front;
    Link<Elem> *rear;
    int size;

public:
    Queue(int sz = DefaultListSize);
    ~Queue() { clear(); }
    void clear();
    bool enqueue(const Elem &);
    bool dequeue(Elem &it);
    bool frontValue(Elem &it) const;
    virtual int length() const { return size; }
};

template <class Elem>
Queue<Elem>::Queue(int sz)
{
    front = nullptr;
    rear = nullptr;
    size = 0;
}

template <class Elem>
void Queue<Elem>::clear()
{
    while (front != nullptr)
    {
        rear = front;
        front = front->next;
        delete rear;
    }
    rear = nullptr;
    size = 0;
}

template <class Elem>
bool Queue<Elem>::enqueue(const Elem &it)
{
    if (rear == nullptr)
    {
        front = rear = new Link<Elem>(it);
    }
    else
    {
        rear->next = new Link<Elem>(it);
        rear = rear->next;
    }
    size++;
    return true;
}

template <class Elem>
bool Queue<Elem>::dequeue(Elem &it)
{
    if (size == 0)
        return false;
    it = front->element;
    Link<Elem> *ltemp = front;
    front = front->next;
    delete ltemp;
    if (front == nullptr)
        rear = nullptr;
    size--;
    return true;
}

template <class Elem>
bool Queue<Elem>::frontValue(Elem &it) const
{
    if (size == 0)
        return false;
    it = front->element;
    return true;
}

#endif