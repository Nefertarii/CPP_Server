#include "tree.h"

using namespace std;

int main()
{
    BinarySearchTree<int> bintree;
    bintree.insert(1);
    bintree.insert(5);
    bintree.insert(3);

    bintree.insert(6);
    bintree.insert(2);
    bintree.insert(4);
    bintree.print_tree();
    return 0;
}