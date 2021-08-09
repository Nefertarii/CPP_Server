#ifndef SORT_LIST_H_
#define SORT_LIST_H_

#include "unsort_List.h"
#include "Compare.h"

template <class Elem>
class List : protected unsortList<Elem>
{
private:
    EECompare<Elem> comp;

public:
    List(int size = DefaultListSize) : unsortList<Elem>(size) {}
    ~List() {}
    using unsortList<Elem>::clear;
    bool insert(const Elem &item);
    using unsortList<Elem>::remove;
    using unsortList<Elem>::setStart;
    using unsortList<Elem>::setEnd;
    using unsortList<Elem>::prev;
    using unsortList<Elem>::next;
    using unsortList<Elem>::leftLength;
    using unsortList<Elem>::rightLength;
    using unsortList<Elem>::setPos;
    using unsortList<Elem>::getValue;
    using unsortList<Elem>::print;
};

template <class Elem>
bool List<Elem>::insert(const Elem &item)
{
    Elem curr;
    for (setStart(); getValue(curr); next())
        if (!comp.lt(curr, item))
            break;
    return unsortList<Elem>::insert(item);
}

#endif