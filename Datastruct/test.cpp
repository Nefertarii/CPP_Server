#include "vector.h"
#include "list.h"
#include <iostream>

using namespace std;

int main()
{
    List<int> A;
    A.push_back(1);
    A.push_back(2);
    A.push_back(3);
    A.pop_back();
    cout << A.size() << " ";
    List<int>::iterator it = A.begin();
    it++;
    cout << *it << "\n";

    return 0;
}