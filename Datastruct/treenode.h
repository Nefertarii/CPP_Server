#ifndef TREENODE_H_
#define TREENODE_H_

#include <utility>

template <typename T>
class treenode
{
public:
    T element_;
    treenode *firstchild;
    treenode *nextsibling;
};

template <typename T>
class binarynode
{
public:
    T element_;
    binarynode *left;
    binarynode *right;
    //binarynode() : element_(0), left(nullptr), right(nullptr){}
    binarynode(const T &element) : element_(element), left(nullptr), right(nullptr){}
    binarynode(const T &element, binarynode *lc, binarynode *rc) : element_(element), left(lc), right(rc) {}
    binarynode(T &&element, binarynode *lc, binarynode *rc) : element_(std::move(element)), left(lc), right(rc){}
};

#endif