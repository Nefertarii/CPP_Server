#include <iostream>
#include <string>

using namespace std;

std::string process2(std::string& str, char stop) {
    size_t size = str.size();
    if (size == 0) { std::string(); }
    size_t index = 0;
    while (1) {
        if (str[index] == stop) {
            std::string tmp(str.begin(), str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        index += 1;
    }
    return std::string();
}

string process(string& str, char stop) {
    size_t size = str.size();
    if (size == 0) { return std::string(); }
    size_t index = 0;
    while (1) {
        index = str.find_first_of(stop);
        if (index >= size) {
            std::string tmp(str.begin() + 1, str.begin() + index);
            str.erase(str.begin(), str.begin() + index + 1);
            return tmp;
        }
        std::cout << "";
        return std::string();
    }
    return std::string();
}

void func1() {
    string log1 = "[] :";
    cout << "process get:"
         << process(log1, ']')
         << "\tlog remaning:" << log1 << "\n";
    cout << "process get:"
         << process(log1, ']')
         << "\tlog remaning:" << log1 << "\n";
    cout << "process get:"
         << process(log1, ':')
         << "\tlog remaning:" << log1 << "\n";
    // std::cout <<""
}

int main() {
    func1();
}