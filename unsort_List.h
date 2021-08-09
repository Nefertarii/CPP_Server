#ifndef UNSORT_LIST_H_
#define UNSORT_LIST_H_

#include "list_T.h"
#include <iostream>

template <class Elem>
class unsortList : public list_T<Elem>
{
private:
    Link<Elem> *head;
    Link<Elem> *tail;
    Link<Elem> *fence;
    int leftcnt;
    int rightcnt;
    void init();
    void removeall();

public:
    unsortList(int size = DefaultListSize) { init(); }
    ~unsortList() { removeall(); }
    void clear();
    bool insert(const Elem &);
    bool append(const Elem &);
    bool remove(Elem &);
    void setStart();
    void setEnd();
    void prev();
    void next();
    int leftLength() const { return leftcnt; }
    int rightLength() const { return rightcnt; }
    bool setPos(int pos);
    bool getValue(Elem &it) const;
    void print() const;
};

template <class Elem>
void unsortList<Elem>::init()
{
    fence = tail = head = new Link<Elem>;
    leftcnt = rightcnt = 0;
}

template <class Elem>
void unsortList<Elem>::removeall()
{
    while (head != nullptr)
    {
        fence = head;
        head = head->next;
        delete fence;
    }
}

template <class Elem>
void unsortList<Elem>::clear()
{
    removeall();
    init();
}

template <class Elem>
bool unsortList<Elem>::insert(const Elem &item)
{
    fence->next = new Link<Elem>(item, fence, fence->next);
    if (fence->next->next != nullptr)
    {
        fence->next->next->prev = fence->next;
    }
    if (tail == fence)
        tail = fence->next;
    rightcnt++;
    return true;
}

template <class Elem>
bool unsortList<Elem>::append(const Elem &item)
{
    tail = tail->next = new Link<Elem>(item, tail, nullptr);
    rightcnt++;
    return true;
}

template <class Elem>
bool unsortList<Elem>::remove(Elem &it)
{
    if (fence->next == nullptr)
        return false;
    it = fence->next->element;
    Link<Elem> *ltemp = fence->next;
    if (ltemp->next != nullptr)
        ltemp->next->prev = fence;
    else
        tail = fence;
    fence->next = ltemp->next;
    delete ltemp;
    rightcnt--;
    return true;
}

template <class Elem>
void unsortList<Elem>::setStart()
{
    fence = head;
    rightcnt += leftcnt;
    leftcnt = 0;
}

template <class Elem>
void unsortList<Elem>::setEnd()
{
    fence = tail;
    leftcnt = rightcnt;
    rightcnt = 0;
}

template <class Elem>
void unsortList<Elem>::prev()
{
    if (fence != head)
    {
        fence = fence->prev;
        leftcnt--;
        rightcnt++;
    }
}

template <class Elem>
void unsortList<Elem>::next()
{
    if (fence != tail)
    {
        fence = fence->next;
        rightcnt--;
        leftcnt++;
    }
}

template <class Elem>
bool unsortList<Elem>::setPos(int pos)
{
    if ((pos < 0) || (pos > rightcnt + leftcnt))
        return false;
    fence = head;
    for (int i = 0; i < pos; i++)
    {
        fence = fence->next;
    }
    return true;
}

template <class Elem>
bool unsortList<Elem>::getValue(Elem &it) const
{
    if (rightLength() == 0)
        return false;
    it = fence->next->element;
    return true;
}

template <class Elem>
void unsortList<Elem>::print() const
{
    Link<Elem> *temp = head;
    std::cout << "< ";
    while (temp != fence)
    {
        std::cout << temp->next->element << " ";
        temp = temp->next;
    }
    std::cout << "| ";
    while (temp->next != nullptr)
    {
        std::cout << temp->next->element << " ";
        temp = temp->next;
    }
    std::cout << ">\n";
}

#endif