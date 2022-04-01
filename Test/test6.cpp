#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using namespace std::placeholders;

void square(double x) {
    std::cout << x << "^2 == " << x * x << std::endl;
}

void product(double x, double y) {
    std::cout << x << "*" << y << " == " << x * y << std::endl;
}

int main() {
    double arg[] = {1, 2, 3};

    std::for_each(&arg[0], &arg[3], square);
    std::cout << std::endl;

    std::for_each(&arg[0], &arg[3], std::bind(product, _1, 2));
    std::cout << std::endl;

    std::for_each(&arg[0], &arg[3], std::bind(square, _1));

    return 0;
}