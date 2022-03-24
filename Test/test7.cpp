#include "../Class/exception.h"
#include "../Log/Base/Head/filehandler.h"
#include "../Log/Base/Head/fontcolor.h"
#include "../Log/Base/Head/logformatter.h"
#include "../Log/Base/Head/loglevel.h"
#include "../Log/Base/Head/logmsg.h"
#include "../Log/Sink/Head/stdsink.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace Wasi;
using namespace Wasi::Log;

void func1(string& name) { cout << "before open " << name << "\n"; }

void func2(string& name, fstream* filestream) {
    cout << "after open " << name << " fstream: " << filestream << "\n";
}

void func3(string& name, fstream* filestream) {
    cout << "before close " << name << " fstream: " << filestream << "\n";
}

void func4(string& name) { cout << "after close " << name << "\n"; }

void T_filehandler_1() {
    FileEvents file_events;
    file_events.before_open  = func1;
    file_events.after_open   = func2;
    file_events.before_close = func3;
    file_events.after_close  = func4;
    FileHandler file_handler(file_events);
    file_handler.Open("test.txt", false);
    file_handler.Write("12345");
}

int main() {
    try {
        T_filehandler_1();
    } catch (Exception& e) {
        cout << e.What() << "\n";
    }
}