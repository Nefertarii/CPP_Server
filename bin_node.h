template <class Elem>
class BinNode
{
public:
    virtual Elem &val() = 0;
    virtual void setVal() = 0;
    virtual BinNode *left() = 0;
    virtual void setLeft(BinNode *) = 0;
    virtual BinNode *right() const = 0;
    virtual void setRight(BinNode *) = 0;
    virtual bool isLeaf() = 0;
};

template <class Elem>
void preorder(BinNode<Elem> *subroot)
{
    if (subroot == nullptr)
        return;
    visit(subroot);
    preorder(subroot->left());
    preorder(subroot->right());
}

template <class Elem>
int count(BinNode<Elem>* subroot)
{
    if(subroot == nullptr)
        return 0;
    return 1 + count(subroot->left()) + count(subroot->right());
}