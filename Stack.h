#ifndef STACK_H_
#define STACK_H_

//Link list-based stack
#include "list_T.h"
template <class Elem>
class Stack
{
private:
    Link<Elem> *top;
    int size;

public:
    Stack(int sz = DefaultListSize);
    ~Stack() { clear(); }
    void clear();
    bool push(const Elem &);
    bool pop(Elem &);
    bool topValue(Elem &) const;
    int length() const { return size; }
};

template <class Elem>
Stack<Elem>::Stack(int sz)
{
    top = nullptr;
    size = 0;
}

template <class Elem>
void Stack<Elem>::clear()
{
    while (top != nullptr)
    {
        Link<Elem> *temp = top;
        top = top->next;
        size = 0;
        delete temp;
    }
}

template <class Elem>
bool Stack<Elem>::push(const Elem &item)
{
    top = new Link<Elem>(item, top);
    size++;
    return true;
}

template <class Elem>
bool Stack<Elem>::pop(Elem &it)
{
    if (size == 0)
        return false;
    it = top->element;
    Link<Elem> *ltemp = top->next;
    delete top;
    top = ltemp;
    size--;
    return true;
}
template <class Elem>
bool Stack<Elem>::topValue(Elem &it) const
{
    if (size == 0)
        return false;
    it = top->element;
    return true;
}

#endif