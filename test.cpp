#include "Queue.h"
#include <iostream>

int main()
{
    Queue<int> A;
    A.enqueue(10);
    A.enqueue(9);

    int it = 0;
    A.dequeue(it);
    std::cout << it << " ";
    return 0;
}