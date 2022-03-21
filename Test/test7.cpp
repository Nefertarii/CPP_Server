#include "../Log/Base/Head/fontcolor.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    // func1();
    std::cout << "RESET      " << RESET << "hello" << RESET << "\n";
    std::cout << "BLACK      " << BLACK << "hello" << RESET << "\n";
    std::cout << "RED        " << RED << "hello" << RESET << "\n";
    std::cout << "GREEN      " << GREEN << "hello" << RESET << "\n";
    std::cout << "YELLOW     " << YELLOW << "hello" << RESET << "\n";
    std::cout << "BLUE       " << BLUE << "hello" << RESET << "\n";
    std::cout << "MAGENTA    " << MAGENTA << "hello" << RESET << "\n";
    std::cout << "CYAN       " << CYAN << "hello" << RESET << "\n";
    std::cout << "WHITE      " << WHITE << "hello" << RESET << "\n";
    std::cout << "BOLDBLACK  " << BOLDBLACK << "hello" << RESET << "\n";
    std::cout << "BOLDRED    " << BOLDRED << "hello" << RESET << "\n";
    std::cout << "BOLDGREEN  " << BOLDGREEN << "hello" << RESET << "\n";
    std::cout << "BOLDYELLOW " << BOLDYELLOW << "hello" << RESET << "\n";
    std::cout << "BOLDBLUE   " << BOLDBLUE << "hello" << RESET << "\n";
    std::cout << "BOLDMAGENTA" << BOLDMAGENTA << "hello" << RESET << "\n";
    std::cout << "BOLDCYAN   " << BOLDCYAN << "hello" << RESET << "\n";
    std::cout << "BOLDWHITE  " << BOLDWHITE << "hello" << RESET << "\n";
}