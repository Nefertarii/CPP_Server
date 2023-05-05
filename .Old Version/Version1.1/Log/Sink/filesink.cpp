#include "filesink.h"
#include <Base/exception.h>
#include <Log/fontcolor.h>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Log;

void FileSink::Fileout() {
    try {
        filehandler.Write(logs);
        suc_count += count;
        count = 0;
        logs.clear();
        filehandler.Close();
    } catch (const Exception& e) {
        std::cerr << e.What() << "\n";
        count = 0;
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
    name = "File Sink";
    LogFormat format;
    format.print_color           = false;
    format.print_source_location = true;
    format.print_thread_id       = true;
    formatter.Set_format(format);
    Init_open(filename);
}

FileSink::FileSink(LogFormat logformat, std::string filename) :
    count(0),
    suc_count(0) {
    name = "File Sink";
    formatter.Set_format(logformat);
    Init_open(filename);
}

FileSink::FileSink(LogFormat logformat, std::string filename, Base::FileEvents events) :
    count(0),
    suc_count(0),
    filehandler(events) {
    name = "File Sink";
    formatter.Set_format(logformat);
    Init_open(filename);
}

void FileSink::Logger(LogMsg& logmsg) {
    formatter.Format(logmsg);
    logs += logmsg.Output();
    ++count;
    if (count % 10 == 0 && count != 1) {
        Fileout();
    }
}

void FileSink::Flush() {
    // logs += "Flush\n";
    // ++count;
    Fileout();
}

void FileSink::Set_format(LogFormat logformat) {
    logformat.print_color = false;
    formatter.Set_format(logformat);
}

uint FileSink::Get_count() { return suc_count; }

FileSink::~FileSink() {
    Fileout();
    // std::cout << "Total save " << suc_count.load() << " message to file:"
    //           << filehandler.Get_file_name() << "\n";
    filehandler.Close();
}