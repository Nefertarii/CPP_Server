#include "Head/vector.h"
#include <thread>
#include <iostream>

thread::Vector<int> ivec;

void func1() {
    for (int i = 0;i < 10;i++) {
        ivec.push_back(i);
    }
}

void func2() {
    while (1) {
        if (ivec.size() < 30) {
            std::this_thread::yield();
        } else {
            for (int i = 0;i < ivec.size();i++) {
                int j = ivec.get(i);
                std::cout << j << " ";
            }
            return;
        }
    }
}


int main() {
    std::thread T1(func1);
    std::thread T2(func1);
    std::thread T3(func1);
    std::thread T4(func2);
    T1.join();
    T2.join();
    T3.join();
    T4.join();
}