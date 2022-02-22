#include "../Headfile/Net/Base/Head/thread.h"
#include <iostream>
#include <unistd.h>

using namespace Wasi::Net;

void func1() {
    std::cout << "tid:" << gettid()
        << " func1\n";
}

void func2(int x) {
    std::cout << "tid:" << gettid()
        << " func2 x:" << x << "\n";
}

void func3() {
    std::cout << "tid:" << gettid()
        << " func3\n";
    sleep(1);
}

class Tclass {
private:
    double x;
public:
    explicit Tclass(double x_) :x(x_) {}
    void t_func1() {
        std::cout << "tid:" << gettid()
            << " Tclass x:" << x << "\n";
    }
    void t_func2(std::string str) {
        std::cout << "tid:" << gettid()
            << " string:" << str<< " Tclass x:" << x << "\n";
    }
};

int main() {
    Thread t1(func1);
    t1.Start();
    t1.Join();
    Thread t2(std::bind(func2, 42));
    t2.Start();
    t2.Join();
    Tclass c1(42.01);
    Thread t3(std::bind(&Tclass::t_func1, &c1));
    t3.Start();
    t3.Join();
    Thread t4(std::bind(&Tclass::t_func2, std::ref(c1), "Wasi"));
    t4.Start();
    t4.Join();
    {
        Thread t5(func3);
        t5.Start();
    }
    sleep(2);
    {
        Thread t6(func3);
        t6.Start();
    }
    sleep(2);
    std::cout << "Thread created:" << Thread::Num_created() << "\n";
}
