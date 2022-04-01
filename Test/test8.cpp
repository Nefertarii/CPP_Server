#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

inline std::string Filename(std::string name) {
    return std::string(name.begin() + name.find_last_of('/'), name.end());
}

int main() {
    cout << Filename(__FILE__);
}
