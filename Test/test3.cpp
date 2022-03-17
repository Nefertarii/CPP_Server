#include "../Net/Base/Head/buffer.h"
#include <iostream>
#include <unistd.h>

using namespace Wasi::Base;

void func1() {
    if (1) {
        Buffer buffer;
        const char* tmp = "hello\n";
        buffer.Append(tmp, 6);
        std::cout << buffer.Content();
        std::cout << "\nSize" << buffer.Size();
    }

}


int main() {
    func1();
}
