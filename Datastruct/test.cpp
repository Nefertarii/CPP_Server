#include "list.h"
#include "stack.h"
#include "vector.h"
#include "queue.h"
#include <iostream>

using namespace std;

int main()
{
    Queue<int> A;
    A.push(1);
    A.push(2);
    A.push(3);
    A.push(4);
    A.pop();
    cout << A.front();
    return 0;
}