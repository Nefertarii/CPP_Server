#ifndef VECTOR_H_
#define VECTOR_H_

#include "list_T.h"
#include <iostream>

template <class Elem>
class Vector : public list_T<Elem>
{
private:
    int maxSize;
    int listSize;
    int fence;
    Elem *listArray;

public:
    Vector(int size = DefaultListSize)
    {
        maxSize = size;
        listSize = fence = 0;
        listArray = new Elem[maxSize];
    }
    ~Vector() { delete[] listArray; }
    void clear();
    bool insert(const Elem &);
    bool append(const Elem &);
    bool remove(Elem &);
    void setStart() { fence = 0; }
    void setEnd() { fence = listSize; }
    void prev();
    void next();
    int leftLength() const { return fence; }
    int rightLength() const { return listSize - fence; }
    bool setPos(int pos);
    bool getValue(Elem &it) const;
    void print() const;
};

template <class Elem>
void Vector<Elem>::clear()
{
    delete[] listArray;
    listSize = fence = 0;
    listArray = new Elem[maxSize];
}

template <class Elem>
bool Vector<Elem>::insert(const Elem &item)
{
    if (listSize == maxSize)
        return false;
    for (int i = listSize; i > fence; i--)
    {
        listArray[i] = listArray[i - 1];
    }
    listArray[fence] = item;
    listSize++;
    return true;
}

template <class Elem>
bool Vector<Elem>::append(const Elem &item)
{
    if (listSize == maxSize)
        return false;
    listArray[listSize++] = item;
    return true;
}

template <class Elem>
bool Vector<Elem>::remove(Elem &it)
{
    if (rightLength() == 0)
        return false;
    it = listArray[fence];
    for (int i = fence; i < listSize - 1; i++)
    {
        listArray[i] = listArray[i + 1];
    }
    listSize--;
    return true;
}

template <class Elem>
void Vector<Elem>::prev()
{
    if (fence != 0)
        fence--;
}

template <class Elem>
void Vector<Elem>::next()
{

    if (fence <= listSize)
        fence++;
}

template <class Elem>
bool Vector<Elem>::setPos(int pos)
{
    if ((pos >= 0) && (pos <= listSize))
        fence = pos;
    return (pos >= 0) && (pos <= listSize);
}

template <class Elem>
bool Vector<Elem>::getValue(Elem &it) const
{
    if (rightLength() == 0)
        return false;
    else
    {
        it = listArray[fence];
        return true;
    }
}

template <class Elem>
void Vector<Elem>::print() const
{
    int temp = 0;
    std::cout << "< ";
    while (temp < fence)
        std::cout << listArray[temp++] << " ";
    std::cout << "| ";
    while (temp < listSize)
        std::cout << listArray[temp++] << " ";
    std::cout << "\n";
}

#endif
