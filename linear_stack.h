#ifndef LINEAR_STACK_H_
#define LINEAR_STACK_H_

#include "list_T.h"

//Array-based stack
template <class Elem>
class linear_Stack
{
private:
    int size;
    int top;
    Elem *listArray;

public:
    linear_Stack(int sz = DefaultListSize);
    ~linear_Stack() { delete[] listArray; }
    void clear() { top = 0; }
    bool push(const Elem &);
    bool pop(Elem &);
    bool topValue(Elem &) const;
    int length() const { return top; }
};

template <class Elem>
linear_Stack<Elem>::linear_Stack(int sz = DefaultListSize)
{
    size = sz;
    top = 0;
    listArray = new Elem[sz];
}

template <class Elem>
bool linear_Stack<Elem>::push(const Elem &item)
{
    if (top == size)
        return false;
    else
    {
        listArray[top++] = item;
        return true;
    }
}

template <class Elem>
bool linear_Stack<Elem>::pop(Elem &it)
{
    if (top == 0)
        return false;
    else
    {
        it = listArray[--top];
        return true;
    }
}

template <class Elem>
bool linear_Stack<Elem>::topValue(Elem &it) const
{
    if (top == 0)
        return false;
    else
    {
        it = listArray[top - 1];
        return true;
    }
}

#endif