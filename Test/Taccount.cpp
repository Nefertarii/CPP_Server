#include <Base/filehandler.h>
#include <Net/Http/httpaccount.h>
#include <iostream>

using namespace Wasi;
using namespace std;

void func1() {
    // fstream file_s("account.txt");
    // if (file_s) {
    //     std::string line1;
    //     getline(file_s, line1);
    //     cout << line1;
    //     file_s.close();
    // }

    Base::FileHandler file;
    file.Open("account.txt");
    std::string line2;
    file.Get_line(line2, 10);
    cout << line2 << "\n";

    Http::HttpAccount accounts("account.txt");
}

int main() {
    func1();
}
