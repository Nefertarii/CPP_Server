#include "Include/Logger/appender.h"
#include "Include/Logger/Sink/filesink.h"
#include "Include/Logger/Sink/consolessink.h"
#include <iostream>

using namespace wasi;
using namespace wasi::log;
using namespace std;

void Sink_t() {
    FileSink sink1("/home/ubuntu/Server/Test/sink_t.txt");
    ConsolesSink sink2;
    string str;
    for (int i = 0; i < 50; ++i) {
        str = to_string(i);
        str += "000000\n";
        sink1.Insert(str);
        sink2.Insert(str);
        cout << sink1.GetCount();
    }
}

void Appender_t_1() {
    std::shared_ptr<ConsolesSink> sink1 = make_shared<ConsolesSink>();
    Appender appender1("appender1");
    appender1.PushBack(sink1);
    appender1.SetFilterLevel(LogLevel::INFO);
    appender1.SetFilterLevel(LogLevel::WARN);
    if (!appender1.Log(LogLevel::INFO, "123123", 123456, "file1", "func2", 3)) {
        cout << "\nthis log is fileter, level INFO\n";
    }
    if (!appender1.Log(LogLevel::WARN, "123123", 123456, "file1", "func2", 3)) {
        cout << "this log is fileter, level WARN\n";
    }
    if (!appender1.Log(LogLevel::DEBUG, "123123", 123456, "file1", "func2", 3)) {
        cout << "this log is fileter, level DEBUG\n";
    }

    appender1.CancelFilterLevel(LogLevel::INFO);
    if (!appender1.Log(LogLevel::INFO, "123123", 123456, "file1", "func2", 3)) {
        cout << "this log is fileter, level INFO\n";
    }
    cout << "\n";
}

void Appender_t_2() {
    std::shared_ptr<FileSink> sink1     = make_shared<FileSink>("/home/ubuntu/Server/Test/sink_t.txt");
    std::shared_ptr<FileSink> sink2     = make_shared<FileSink>("/home/ubuntu/Server/Test/sink_t_2.txt");
    std::shared_ptr<ConsolesSink> sink3 = make_shared<ConsolesSink>();
    Appender appender1("appender1");
    appender1.PushBack(sink1);
    appender1.PushBack(sink2);
    appender1.PushBack(sink3);
    LoggerSetting setting;
    setting.print_color           = false;
    LogFormatter formatter;
    formatter.SetFormat(setting);
    appender1.SetFormatter(formatter);
    vector<string> sink_name_list = appender1.GetListName();
    cout << "Now has " << appender1.Size() << "sinks\n";
    for (auto i : sink_name_list) {
        cout << i << "\n";
    }
    appender1.Log(LogLevel::INFO, "123123", 123456, "file1", "func2", 3);
    cout << "\n";
}

int main() {
    // Sink_t();
    // Appender_t_1();
    Appender_t_2();
}