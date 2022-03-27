#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class A {
public:
    // A() { cout << "Create A\n"; }
    virtual void Print() = 0;
    virtual ~A()         = default;
};

class B : public A {
public:
    B(char b) { cout << "Create B\n"; }
    void Print() { cout << "B\n"; }
    ~B() override { cout << "Earse B\n"; }
};

class C : public A {
public:
    C(int a) { cout << "Create C\n"; }
    void Print() { cout << "B\n"; }
    ~C() override { cout << "Earse B\n"; }
};

int main() {
    A* a;
    B b('b');
    a = &b;
    a->Print();
    C c(1);
}
