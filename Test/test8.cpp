#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

int main() {
    int a        = 1;
    int* p       = &a;
    string str_p = (const char*)p;
    cout << str_p;
}
