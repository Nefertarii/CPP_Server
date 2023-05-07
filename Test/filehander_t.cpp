#include "Include/Header/filehandler.h"
#include <iostream>

using namespace wasi;
using namespace wasi::head;
using namespace std;

void filehandler_t() {
    FileHandler file;
    file.Open("/home/ubuntu/Server/setting");
    string read_str;
    file.Read(read_str);
    cout << read_str << "\n";
}

int main() {
    filehandler_t();
}