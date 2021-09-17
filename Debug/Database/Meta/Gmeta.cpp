#include "Metaconfirm.h"

using namespace std;
int main(int argc, const char* argv[]) {
    std::string file;
    if (argc != 2) {
        file = "/home/cs18/vscode/Webserver/Accountinfo.txt";
    } else {
        file = argv[1];
    }
    Meta_Parse T(file);
    T.SetLog(nullptr, 200);

}