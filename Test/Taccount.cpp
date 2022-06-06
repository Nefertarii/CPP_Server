#include <Base/Timer/clock.h>
#include <Base/filehandler.h>
#include <Net/Http/httpaccount.h>
#include <fstream>
#include <iostream>
#include <thread>

using namespace Wasi;
using namespace std;

void read_write_test() {
    Base::FileHandler file;
    std::string tmp;
    file.Open("account.txt");
    file.Read(tmp, 140, 14);
    cout << tmp << "\n";

    file.Write("add a new line\n");
}

void get_line_test() {
    Base::FileHandler file;
    std::string tmp;
    file.Open("account.txt");
    file.Get_line(tmp, 10);
    cout << tmp << "\n";
}

void find_test() {
    Base::FileHandler file;
    std::string tmp;
    file.Open("account.txt");
    int beg_time = 0, end_time = 0;

    beg_time             = Time::Clock::Nowtime_ms();
    std::string find_str = "hncsgwc@gmail.com";
    size_t posi          = file.Find(find_str);
    if (posi != std::string::npos) {
        cout << "find success, posi: " << posi << "\n";
    }
    end_time = Time::Clock::Nowtime_ms();
    cout << "use " << end_time - beg_time << " ms\n";
    file.Read(tmp, posi, find_str.size());
    cout << tmp << "\n";

    std::string change_str = "hncsgwc@qq.com   ";
    file.Write(change_str, posi);

    file.Read(tmp, posi, find_str.size());
    cout << tmp << "\n";
}

int main() {
    // read_write_test();
    find_test();
    // get_line_test();
}
