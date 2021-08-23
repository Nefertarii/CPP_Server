#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include "treenode.h"
#include <iostream>

/*
 * BinarySearch tree 
 * for each node T his left subtree value all less than T
 * and his right subtree value all greater than T
 */
template <typename T>
class BinarySearchTree
{
private:
    binarynode<T> *root;
    void insert(const T &value, binarynode<T> *&t);
    void insert(T &&value, binarynode<T> *&t);
    void remove(const T &value, binarynode<T> *&t);
    binarynode<T> *findmin(binarynode<T> *t) const;
    binarynode<T> *findmax(binarynode<T> *t) const;
    bool contains(const T &x, binarynode<T> *t) const;
    void makeempty(binarynode<T> *&t);
    void printtree(binarynode<T> *t, std::ostream &out);
    binarynode<T> *clone(binarynode<T> *t) const;

public:
    BinarySearchTree() : root(nullptr){};
    BinarySearchTree(const BinarySearchTree &rhs) : root(nullptr) { root = clone(rhs.root); }
    const T &findmin() const;
    const T &findmax() const;
    bool contains(const T &x) const { return contains(x, root); }
    bool isempty() const { return root == nullptr; }
    void print_tree(std::ostream &out = std::cout) { printtree(root, out); }
    void make_empty() { makeempty(root); }
    void insert(const T &value) { insert(value, root); }
    void insert(T &&value) { insert(value, root); }
    void remove(const T &value) { remove(value, root); }
    //BinarySearchTree &operator=(const BinarySearchTree &rhs);
    //BinarySearchTree &operator=(BinarySearchTree &&rhs);
    ~BinarySearchTree() { make_empty(); }
};

template <typename T>
bool BinarySearchTree<T>::contains(const T &value, binarynode<T> *t) const
{
    if (t == nullptr)
        return false;
    else if (value < t->element_)
        return contains(value, t->left);
    else if (t->element_ < value)
        return contains(value, t->right);
    else
        return true;
}

template <typename T>
binarynode<T> *BinarySearchTree<T>::findmin(binarynode<T> *t) const
{
    if (t == nullptr)
        return nullptr;
    if (t->left == nullptr)
        return t;
    return findmin(t->left);
}

template <typename T>
binarynode<T> *BinarySearchTree<T>::findmax(binarynode<T> *t) const
{
    if (t != nullptr)
    {
        while (t->right != nullptr)
        {
            t = t->right;
        }
    }
    return t;
}

template <typename T>
void BinarySearchTree<T>::insert(const T &value, binarynode<T> *&t)
{
    if (t == nullptr)
        t = new binarynode<T>(value, nullptr, nullptr);
    else if (value < t->element_)
        insert(value, t->left);
    else if (t->element_ < value)
        insert(value, t->right);
    else
        ;
}

template <typename T>
void BinarySearchTree<T>::insert(T &&value, binarynode<T> *&t)
{
    if (t == nullptr)
        t = new binarynode<T>(std::move(value), nullptr, nullptr);
    else if (value < t->element_)
        insert(std::move(value), t->left);
    else if (t->element_ < value)
        insert(std::move(value), t->right);
    else
        ;
}

template <typename T>
void BinarySearchTree<T>::remove(const T &value, binarynode<T> *&t)
{
    if (t == nullptr)
        return;
    else if (value < t->element_)
        remove(value, t->left);
    else if (t->element_ < value)
        remove(value, t->right);
    else if (t->left != nullptr && t->right != nullptr)
    {
        t->element_ = findmin(t->right)->element_;
        remove(t->element_, t->right);
    }
    else
    {
        binarynode<T> *old = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete old;
    }
}

template <typename T>
void BinarySearchTree<T>::makeempty(binarynode<T> *&t)
{
    if (t != nullptr)
    {
        makeempty(t->left);
        makeempty(t->right);
        delete t;
    }
    t = nullptr;
}

template <typename T>
binarynode<T> *BinarySearchTree<T>::clone(binarynode<T> *t) const
{
    if (t == nullptr)
        return nullptr;
    else
        return new binarynode(t->element_, clone(t->left), clone(t->right));
}

template <typename T>
void BinarySearchTree<T>::printtree(binarynode<T> *t, std::ostream &out)
{
    if (t == nullptr)
    {
        return;
    }
    out << t->element_ << " ";
    printtree(t->left, out);
    printtree(t->right, out);
}

template <typename T>
const T &BinarySearchTree<T>::findmin() const
{
    binarynode<T> *ret = findmin(root);
    return ret->element_;
}
template <typename T>
const T &BinarySearchTree<T>::findmax() const
{
    binarynode<T> *ret = findmax(root);
    return ret->element_;
}

/*template <typename T>
BinarySearchTree &BinarySearchTree<T>::operator=(const BinarySearchTree &rhs)

template <typename T>
BinarySearchTree &BinarySearchTree<T>::operator=(BinarySearchTree &&rhs)*/

#endif