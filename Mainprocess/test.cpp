#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void func(int sig) {
    cout << sig << "\n";
    exit(sig);
}

void nothing(void *function) {
    ;
}

int main() {
    signal
    nothing(func, 2);
}