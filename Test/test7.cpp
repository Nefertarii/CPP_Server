#include "../Class/exception.h"
#include "../Log/Base/Head/fontcolor.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;
using namespace Wasi;

void func1() {
    throw Exception();
}

int main() {
    // try {
    //     func1();
    // } catch (const Exception& e) {
    //     cerr << e.What() << '\n';
    // }
    fstream file;
    file.open("test.txt", ios::binary | ios::out | ios::ate);
    if (file.is_open()) {
        cout << "open\n";
    } else {
        cout << strerror(errno) << "\n";
    }
    std::cout << file.write("12345", 5) << "\n";
    if (file.bad()) {
        cout << "write fail\n";
    }
    file.close();
    fstream* file2 = &file;
    file2->open("test.txt", ios::binary | ios::in);
    std::string str;
    char str2[1024];
    while (file2->getline(str2, sizeof(str2))) {
        cout << str2 << "\n";
    }

    // func1();
    //    std::cout << "RESET      " << RESET << "hello" << RESET << "\n";
    //    std::cout << "BLACK      " << BLACK << "hello" << RESET << "\n";
    //    std::cout << "RED        " << RED << "hello" << RESET << "\n";
    //    std::cout << "GREEN      " << GREEN << "hello" << RESET << "\n";
    //    std::cout << "YELLOW     " << YELLOW << "hello" << RESET << "\n";
    //    std::cout << "BLUE       " << BLUE << "hello" << RESET << "\n";
    //    std::cout << "MAGENTA    " << MAGENTA << "hello" << RESET << "\n";
    //    std::cout << "CYAN       " << CYAN << "hello" << RESET << "\n";
    //    std::cout << "WHITE      " << WHITE << "hello" << RESET << "\n";
    //    std::cout << "BOLDBLACK  " << BOLDBLACK << "hello" << RESET << "\n";
    //    std::cout << "BOLDRED    " << BOLDRED << "hello" << RESET << "\n";
    //    std::cout << "BOLDGREEN  " << BOLDGREEN << "hello" << RESET << "\n";
    //    std::cout << "BOLDYELLOW " << BOLDYELLOW << "hello" << RESET << "\n";
    //    std::cout << "BOLDBLUE   " << BOLDBLUE << "hello" << RESET << "\n";
    //    std::cout << "BOLDMAGENTA" << BOLDMAGENTA << "hello" << RESET << "\n";
    //    std::cout << "BOLDCYAN   " << BOLDCYAN << "hello" << RESET << "\n";
    //    std::cout << "BOLDWHITE  " << BOLDWHITE << "hello" << RESET << "\n";

    //    cout << "Line 1...";
    //    usleep(500000);
    //    cout << "\nLine 2" << endl;
    //    cout << "Line 3" << endl;
    //    return 0;
}