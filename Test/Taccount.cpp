#include <Net/Http/httpaccount.h>
#include <iostream>

using namespace Wasi;
using namespace std;

void func1() {
    Http::HttpAccount account("");
}

int main() {
    func1();
}
