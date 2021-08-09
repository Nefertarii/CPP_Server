#include "vector.h"
#include <iostream>

using namespace std;

int main()
{
    Vector<int> A;
    A.push_back(1);
    A.push_back(2);
    A.push_back(3);
    A.pop_back();
    cout << A[0] << " ";
    Vector<int>::iterator it = A.begin();
    it++;
    cout << *it << "\n";

    return 0;
}