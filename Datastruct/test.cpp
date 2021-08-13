#include "tree.h"

using namespace std;

int main()
{
    AVLTree<int> avltree;
    avltree.insert(1);
    avltree.insert(2);
    avltree.insert(3);
    avltree.insert(44);
    avltree.insert(31);
    avltree.insert(42);
    avltree.insert(53);
    avltree.insert(41);
    avltree.insert(63);
    avltree.insert(421);
    avltree.insert(5);
    avltree.insert(6);
    avltree.preorder_print();
    cout << "\n";
    avltree.inorder_print();
    cout << "\n";
    avltree.postorder_print();
    cout << "\n";
    avltree.remove(44);
    avltree.preorder_print();
    cout << "\n";
    avltree.inorder_print();
    cout << "\n";
    avltree.postorder_print();
    cout << "\n";
    return 0;
}