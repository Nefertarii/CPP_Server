#ifndef MAP_H_
#define MAP_H_

#include "List.h"
#include "dictionary.h"

//This map with sorted array-based list
template <class Key, class Elem>
class Map : public Dictionary<Key, Elem>
{
private:
    List<Elem> *list;
    EECompare<Elem> EEComp;
    KECompare<Key, Elem> KEComp;

public:
    Map(int size = DefaultListSize) { list = new List<Elem>(size); }
    ~Map() { delete list; }
    void clear() { list->clear(); }
    bool insert(const Elem &e) { return list->insert(e); }
    bool remove(const Key &K, Elem &e);
    bool removeAny(Elem &e);
    bool find(const Key &K, Elem &e) const;
    int size() { return list->leftLength() + list->rightLength(); }
};

template <class Key, class Elem>
bool Map<Key, Elem>::remove(const Key &K, Elem &e)
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
bool Map<Key, Elem>::removeAny(Elem &e)
{
    if (size() == 0)
        return false;
    //list->setEnd();
    list->prev();
    list->remove(e);
    return true;
}

template <class Key, class Elem>
bool Map<Key, Elem>::find(const Key &K, Elem &e) const
{
    int l = -1;
    int r = list->leftLength() + list->rightLength();
    while (l + 1 != r)
    {
        int i = (l + r) / 2;
        list->setPos(i);
        list->getValue(e);
        if (KEComp.lt(K, e))
            r = i;
        if (KEComp.eq(K, e))
            return true;
        if (KEComp.gt(K, e))
            l = i;
    }
    return false;
}

#endif