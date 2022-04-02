#include "../Thread/Head/threadpool.h"
//#include "../Thread/.old/threadpool.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <cassert>

using namespace std;

void func1() {
    cout << 1 << '\n';
}

int main() {
    // Wasi::Base::ThreadPool tp;
    Wasi::Base::ThreadPool tp;
    for (int i = 0; i < 100; i++) {
        tp.Submit(func1);
    }
    tp.Run_pending_task();
}