#ifndef COMPARE_H_
#define COMPARE_H_

template <class Elem>
class EECompare
{
public:
    EECompare() {}
    ~EECompare() {}
    static bool lt(const Elem A, const Elem B) { return A < B; }
    static bool eq(const Elem A, const Elem B) { return A == B; }
    static bool gt(const Elem A, const Elem B) { return A > B; }
};

template <class Key, class Elem>
class KECompare
{
public:
    KECompare() {}
    ~KECompare() {}
    static bool lt(const Key A, const Elem B) { return A < B; }
    static bool eq(const Key A, const Elem B) { return A == B; }
    static bool gt(const Key A, const Elem B) { return A > B; }
};

#endif