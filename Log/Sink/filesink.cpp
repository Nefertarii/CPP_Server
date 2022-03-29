#include "Head/filesink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi;
using namespace Wasi::Log;

void FileSink::Fileout() {
    std::lock_guard<std::mutex> lk(mtx);
    try {
        filehandler.Reopen();
        filehandler.Write(logs);
        suc_count.fetch_add(count.load());
        count.store(0);
        logs.clear();
        filehandler.Close();
    } catch (const Exception& e) {
        std::cerr << e.What();
        count.store(0);
        filehandler.Close();
    }
}

void FileSink::Init_open(std::string filename) {
    try {
        filehandler.Open(filename, false);
        filehandler.Close();
    } catch (const Exception& e) {
        filehandler.Create(filename);
    }
}

FileSink::FileSink(std::string filename) :
    count(0),
    suc_count(0) {
    LogFormat format;
    format.print_color = false;
    formatter.Set_format(format);
    Init_open(filename);
}

FileSink::FileSink(LogFormat logformat, std::string filename) :
    count(0),
    suc_count(0) {
    formatter.Set_format(logformat);
    Init_open(filename);
}

FileSink::FileSink(LogFormat logformat, std::string filename, FileEvents events) :
    count(0),
    suc_count(0),
    filehandler(events) {
    formatter.Set_format(logformat);
    Init_open(filename);
}

void FileSink::Logger(LogMsg& logmsg) {
    formatter.Format(logmsg);
    logs += logmsg.Output();
    count.fetch_add(1);
    if (count % 10 == 0 && count != 1) {
        Fileout();
    }
}

void FileSink::Flush() {
    logs += "Flush\n";
    count.fetch_add(1);
    Fileout();
}

void FileSink::Set_format(LogFormat logformat) {
    logformat.print_color = false;
    formatter.Set_format(logformat);
}

uint FileSink::Get_count() { return suc_count.load(); }

FileSink::~FileSink() {
    Fileout();
    // std::cout << "Total save " << suc_count.load() << " message to file:"
    //           << filehandler.Get_file_name() << "\n";
    filehandler.Close();
}