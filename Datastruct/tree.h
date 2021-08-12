#ifndef TREE_H_
#define TREE_H_

#include "treenode.h"
#include <iostream>

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
    BinarySearchTree(BinarySearchTree &&rhs);
    const T &findmin() const { return findmin(root); }
    const T &findmax() const { return findmax(root); }
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
        return new binarynode(t->element_, clone(t->left), clonde(t->right));
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

/*template <typename T>
BinarySearchTree &BinarySearchTree<T>::operator=(const BinarySearchTree &rhs)

template <typename T>
BinarySearchTree &BinarySearchTree<T>::operator=(BinarySearchTree &&rhs)*/

template <typename T>
class AvlTree
{
private:
    static const int ALLOWED_IMBALANCE = 1;
    void insert(const T &value, avlnode *&t);
    void balance(avlnode *&t);
    void rotate_withleftchild(avlnode *&k2);
    void rotate_withrightchild(avlnode *&k2);
    void double_withleftchild(avlnode *&k3);
    void remove(const T &x, avlnode *&t);

public:
};
template <typename T>
void AvlTree::insert(const T &value, avlnode<T> *&t)
{
    if (t == nullptr)
        t = new avlnode(value, nullptr, nullptr);
    else if (value < t->element_)
        insert(value, t->left);
    else
        (t->element_ < value)
            insert(value, t->right);
    balance(t);
}

template <typename T>
void AvlTree::balance(avlnode<T> *&t)
{
    if (t == nullptr)
        return;
    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
        if (height(t->right->right) >= height(t->right->left))
            rotate_withleftchild(t);
        else
            double_withleftchild(t);
    else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
        if (height(t->right->right) >= height(t->right->left))
            rotate_withrightchild(t);
        else
            double_withleftchild(t);
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <typename T>
void AvlTree::rotate_withleftchild(avlnode<T> *&k2)
{
    avlnode<T> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <typename T>
void AvlTree::rotate_withrightchild(avlnode < Tleft * &k2)
{
    avlnode<T> *k1 = k2->right;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->right), k2->height) + 1;
    k2 = k1;
}

template <typename T>
void AvlTree::double_withleftchild(avlnode *&k3)
{
    rotate_withrightchild(k3->left);
    rotate_withleftchild(k3);
}

template <typename T>
void AvlTree::remove(const T &value, avlnode<T> *&t)
{
    if (t == nullptr)
        return;
    if (value < t->element_)
        remove(value, t->left);
    else if (t->element_ < value)
        remove(value, t->right);
    else if (t->left != nullptr && t->right != nullptr)
    {
        t->element_ = findmin(t->right)->element;
        remove(t->element, t->right);
    }
    else
    {
        avlnode<T> *old = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete old;
    }
    balance(t);
}

#endif