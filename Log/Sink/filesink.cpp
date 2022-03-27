#include "Head/filesink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi;
using namespace Wasi::Log;

void FileSink::Fileout() {
    mtx.lock();
    try {
        file.Write(logs);
    } catch (const Exception& e) {
        std::cerr << e.What();
    }
    mtx.unlock();
}

FileSink::FileSink(LogFormat logformat, std::string filename) :
    count(0),
    logs(std::string()) {
    formatter->Set_format(logformat);
    try {
        filehandler.Open(filename, false);
        filehandler.Close();
    } catch (const Exception& e) {
        filehandler.Create(filename);
    }
}

FileSink::FileSink(LogFormat logformat, std::string filename, FileEvents events) :
    count(0),
    logs(std::string()),
    file(events) {
    formatter->Set_format(logformat);
    try {
        filehandler.Open(filename, false);
        filehandler.Close();
    } catch (const Exception& e) {
        filehandler.Create(filename);
    }
}

void FileSink::Logger(LogMsg& logmsg) {
    formatter.Format(logmsg);
    logs += logmsg.Output();
    count.fetch_add();
    if (count % 10 == 0) {
        Fileout();
    }
}

void FileSink::Flush() {
    logs += "Flush\n";
    Fileout();
}

void FileSink::Set_format(LogFormat logformat) { formatter->Set_format(logformat); }

uint FileSink::Get_count() { return count.load(); }

FileSink::~FileSink() {
    Fileout();
    std::cout << "Total save " << count.load() << " message to file:" << file.Get_file_name() << "\n";
    file.Close();
}