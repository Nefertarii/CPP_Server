#include "../../Important/Gfilefunc.h"
#include <algorithm>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>




int main() {
    using namespace std;
    char* str;
    str = getpass("input:");
    cout << str;
}