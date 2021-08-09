#ifndef LIST_T_H_
#define LIST_T_H_

#include <cstddef>

#define DefaultListSize 10

//list or vector template;
template <class Elem>
class list_T
{
public:
    virtual void clear() = 0;
    virtual bool insert(const Elem &) = 0;
    virtual bool append(const Elem &) = 0;
    virtual bool remove(Elem &) = 0;
    virtual void setStart() = 0;
    virtual void setEnd() = 0;
    virtual void prev() = 0;
    virtual void next() = 0;
    virtual int leftLength() const = 0;
    virtual int rightLength() const = 0;
    virtual bool setPos(int pos) = 0;
    virtual bool getValue(Elem &) const = 0;
    virtual void print() const = 0;
};

bool find(list_T<int> &L, int K)
{
    int it;
    for (L.setStart(); L.getValue(it); L.next())
    {
        if (K == it)
            return true;
    }
    return false;
}

//list node
template <class Elem>
class Link
{
private:
    static Link<Elem> *freelist;

public:
    Elem element;
    Link *next;
    Link *prev;
    Link(const Elem &elemval, Link *prevp = nullptr, Link *nextp = nullptr)
    {
        element = elemval;
        prev = prevp;
        next = nextp;
    }
    Link(Link *nextval = nullptr) { next = nextval; }
    void *operator new(size_t);
    void operator delete(void *);
};

template <class Elem>
Link<Elem> *Link<Elem>::freelist = nullptr;

template <class Elem>
void *Link<Elem>::operator new(size_t)
{
    if (freelist == nullptr)
        return ::new Link;
    Link<Elem> *temp = freelist;
    freelist = freelist->next;
    return temp;
}

template <class Elem>
void Link<Elem>::operator delete(void *ptr)
{
    ((Link<Elem> *)ptr)->next = freelist;
    freelist = (Link<Elem> *)ptr;
}

#endif