#include <iostream>
#include <bitset>

enum Tnums {
    E = 0x01,
    D = 0x02,
    C = 0x04,
    B = 0x08,
    A = 0x10
};
void func6(int num) {
    int count = 0;
    while (num) {
        num = num & (num - 1);
        std::cout << "1";
        //} else { std::cout << "0"; }
    }
}


int main() {
    //func1();
    //func4();
    Tnums T1 = A;
    Tnums T2 = C;
    std::bitset<5> bit1(T1 | T2);
    std::cout << bit1.to_string() << "\n";
}