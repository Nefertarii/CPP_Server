#ifndef QUEUE_T_H_
#define QUEUE_T_H_

#include <cstddef>

#define DefaultListSize 10

template <class Elem>
class queue_T
{
public:
    virtual void clear() = 0;
    virtual bool enqueue(const Elem &) = 0;
    virtual bool dequeue(Elem &) = 0;
    virtual bool frontValue(Elem &) const = 0;
    virtual int length() const = 0;
};

#endif