#ifndef UNSORT_MAP_H_
#define UNSORT_MAP_H_

#include "Compare.h"
#include "Vector.h"
#include "dictionary.h"

//This map with sorted array-based list
template <class Key, class Elem>
class unsort_Map : public Dictionary<Key, Elem>
{
private:
    Vector<Elem> *list;
    EECompare<Elem> EEComp;
    KECompare<Key, Elem> KEComp;

public:
    unsort_Map(int size = DefaultListSize) { list = new Vector<Elem>(size); }
    ~unsort_Map() { delete list; }
    void clear() { list->clear(); }
    bool insert(const Elem &e) { return list->append(e); }
    bool remove(const Key &K, Elem &e);
    bool removeAny(Elem &e);
    bool find(const Key &K, Elem &e) const;
    int size() { return list->leftLength() + list->rightLength(); }
};

template <class Key, class Elem>
bool unsort_Map<Key, Elem>::remove(const Key &K, Elem &e)
{
    for (list->setStart(); list->getValue(e); list->next())
    {
        if (KEComp.eq(K, e))
        {
            list->remove(e);
            return true;
        }
    }
    return false;
}

template <class Key, class Elem>
bool unsort_Map<Key, Elem>::removeAny(Elem &e)
{
    if (size() == 0)
        return false;
    list->setEnd();
    list->prev();
    list->remove(e);
    return true;
}

template <class Key, class Elem>
bool unsort_Map<Key, Elem>::find(const Key &K, Elem &e) const
{
    for (list->setStart(); list->getValue(e); list->next())
    {
        if (KEComp.eq(K, e))
            return true;
    }
    return false;
}

#endif