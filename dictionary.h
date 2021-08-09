#ifndef DICTIONARY_H_
#define DICTIONARY_H_

//KEcomp compares a key and an element.
//EEcomp compares two elements.
template <class Key, class Elem>
class Dictionary
{
public:
    virtual void clear() = 0;
    virtual bool insert(const Elem &) = 0;
    virtual bool remove(const Key &, Elem &) = 0;
    virtual bool removeAny(Elem &) = 0;
    virtual bool find(const Key &, Elem &) const = 0;
    virtual int size() = 0;
};



#endif