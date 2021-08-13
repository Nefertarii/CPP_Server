#ifndef TREE_H_
#define TREE_H_

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

/*
 * AVL tree
 * BinarySearch tree with balance condition 
 * The height of the left and right subtree of 
 * each node differs by at most one
 */
template <typename T>
class AVLTree
{
private:
    static const int ALLOWED_IMBALANCE = 1;
    inline int height(avlnode<T> *t) const { return t == nullptr ? -1 : t->height; }
    inline int max(int A, int B) const { return A > B ? A : B; }
    void insert(const T &value, avlnode<T> *&t);
    void remove(const T &value, avlnode<T> *&t);
    void balance(avlnode<T> *&t);
    void rotate_withleftchild(avlnode<T> *&k2);
    void rotate_withrightchild(avlnode<T> *&k2);
    void double_withleftchild(avlnode<T> *&k3);
    void double_withrightchild(avlnode<T> *&k1);
    void makeempty(avlnode<T> *&t);
    void preorderprint(avlnode<T> *t,std::ostream &out);
    void inorderprint(avlnode<T> *t,std::ostream &out);
    void postorderprint(avlnode<T> *t,std::ostream &out);
    avlnode<T> *findmin(avlnode<T> *t) const;
    avlnode<T> *findmax(avlnode<T> *t) const;
    avlnode<T> *clone(avlnode<T> *t) const;
    avlnode<T> *root;

public:
    AVLTree() : root(nullptr){};
    AVLTree(const AVLTree &rhs) : root(nullptr) { root = clone(rhs.root); }
    const T &findmin() const;
    const T &findmax() const;
    bool isempty() const { return root == nullptr; }
    void preorder_print(std::ostream &out = std::cout) { preorderprint(root, out); }
    void inorder_print(std::ostream &out = std::cout) { inorderprint(root, out); }
    void postorder_print(std::ostream &out = std::cout) { postorderprint(root, out); }
    void make_empty() { makeempty(root); }
    void insert(const T &value) { insert(value, root); }
    void remove(const T &value) { remove(value, root); }
    //BinarySearchTree &operator=(const BinarySearchTree &rhs);
    //BinarySearchTree &operator=(BinarySearchTree &&rhs);
    //~AVLTree() { make_empty(); }
};
template <typename T>
void AVLTree<T>::insert(const T &value, avlnode<T> *&t)
{
    if (t == nullptr)
        t = new avlnode<T>(value, nullptr, nullptr);
    else if (value < t->element_)
        insert(value, t->left);
    else if (t->element_ < value)
        insert(value, t->right);
    balance(t);
}

template <typename T>
void AVLTree<T>::balance(avlnode<T> *&t)
{
    if (t == nullptr)
        return;
    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
    {
        if (height(t->left->left) >= height(t->left->right))
            rotate_withleftchild(t);
        else
            double_withleftchild(t);
    }
    else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
    {
        if (height(t->right->right) >= height(t->right->left))
            rotate_withrightchild(t);
        else
            double_withrightchild(t);
    }
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <typename T>
void AVLTree<T>::rotate_withleftchild(avlnode<T> *&k2)
{
    avlnode<T> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k1->height) + 1;
    k2 = k1;
}

template <typename T>
void AVLTree<T>::rotate_withrightchild(avlnode<T> *&k1)
{
    avlnode<T> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k2->height) + 1;
    k1 = k2;
}

template <typename T>
void AVLTree<T>::double_withleftchild(avlnode<T> *&k3)
{
    rotate_withrightchild(k3->left);
    rotate_withleftchild(k3);
}

template <typename T>
void AVLTree<T>::double_withrightchild(avlnode<T> *&k1)
{
    rotate_withleftchild(k1->right);
    rotate_withrightchild(k1);
}

template <typename T>
void AVLTree<T>::remove(const T &value, avlnode<T> *&t)
{
    if (t == nullptr)
        return;
    if (value < t->element_)
        remove(value, t->left);
    else if (t->element_ < value)
        remove(value, t->right);
    else if (t->left != nullptr && t->right != nullptr)
    {
        avlnode<T> *tmp = findmin(t->right);
        t->element_ = tmp->element_;
        remove(t->element_, t->right);
    }
    else
    {
        avlnode<T> *old = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete old;
    }
    balance(t);
}

template <typename T>
void AVLTree<T>::makeempty(avlnode<T> *&t)
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
void AVLTree<T>::preorderprint(avlnode<T> *t,std::ostream &out)
{
    if (t == nullptr)
    {
        return;
    }
    out << t->element_ << " ";
    preorderprint(t->left, out);
    preorderprint(t->right, out);
}
template <typename T>
void AVLTree<T>::inorderprint(avlnode<T> *t,std::ostream &out)
{
    if (t == nullptr)
    {
        return;
    }
    inorderprint(t->left, out);
    out << t->element_ << " ";
    inorderprint(t->right, out);
}
template <typename T>
void AVLTree<T>::postorderprint(avlnode<T> *t,std::ostream &out)
{
    if (t == nullptr)
    {
        return;
    }
    postorderprint(t->left, out);
    postorderprint(t->right, out);
    out << t->element_ << " ";
}

template <typename T>
avlnode<T> *AVLTree<T>::findmin(avlnode<T> *t) const
{
    if (t == nullptr)
        return nullptr;
    while (t->left != nullptr)
        t = t->left;
    return t;
}

template <typename T>
avlnode<T> *AVLTree<T>::findmax(avlnode<T> *t) const
{
    if (t == nullptr)
        return nullptr;
    while (t->right != nullptr)
        t = t->right;
    return t;
}

template <typename T>
const T &AVLTree<T>::findmin() const
{
    avlnode<T> ret = findmin(root);
    return ret.element_;
}

template <typename T>
const T &AVLTree<T>::findmax() const
{
    avlnode<T> ret = findmax(root);
    return ret.element_;
}

template <typename T>
avlnode<T> *AVLTree<T>::clone(avlnode<T> *t) const
{
    if (root != nullptr)
        return new avlnode<T>(root->element_, clone(root->left), clone(root->right));
    else
        return nullptr;
}

#endif