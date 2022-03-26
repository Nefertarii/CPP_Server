#include "Head/filesink.h"
#include "../../Class/exception.h"
#include "../Base/Head/fontcolor.h"
#include <iostream>

using namespace Wasi;
using namespace Wasi::Log;

void FileSink::Fileout() {
    std::lock_guard<std::mutex> lk(mtx);
    if (filter_out == LogLevel::UNINITIALIZED || filter_out == LogLevel::NONE) {
        logcount.fetch_add(1);
        logvec.push_back(logline);
    } else if (String_to_Level(logline.Get_level()) != filter_out) {
        return;
    } else {
        logcount.fetch_add(1);
        logvec.push_back(logline);
    }
    if (logvec.size() >= max_buffer) {
        Save_to_file();
    }
}

void FileSink::Save_to_file() {
    try {
        filehandler.Reopen(false);
        std::string tmp_logmsg;
        tmp_logmsg.clear();
        for (uint i = 0; i < logvec.size(); ++i) {
            tmp_logmsg += logvec[i].Output();
            if (i % 10 == 0) {
                filehandler.Write(tmp_logmsg);
                tmp_logmsg.clear();
            }
        }
        logvec.erase(logvec.begin(), logvec.end());
        filehandler.Write(tmp_logmsg);
        filehandler.Close();
    } catch (const Exception& e) {
        std::cerr << e.What();
    }
}

FileSink::FileSink(LogFormat logformat) :
    filehandler(FileEvents()),
    filter_in(LogLevel::UNINITIALIZED),
    filter_out(LogLevel::UNINITIALIZED),
    max_buffer(200),
    log_filename("test.txt"),
    logcount(0) {
    formatter.Set_format(logformat);
    try {
        filehandler.Open(log_filename, false);
        filehandler.Close();
    } catch (const Exception& e) {
        filehandler.Create(log_filename);
    }
}

void FileSink::Logger(LogMsg logmsg) {
    logline = logmsg;
    formatter.Format(logline);
    if (filter_in == LogLevel::UNINITIALIZED || filter_in == LogLevel::NONE) {
        Fileout();
    } else if (String_to_Level(logline.Get_level()) <= filter_in) {
        return;
    } else {
        Fileout();
    }
}

void FileSink::Flush() {
    Fileout();
    std::cout << "flush" << std::endl;
}

void FileSink::Set_format(LogFormat logformat) { formatter.Set_format(logformat); }

void FileSink::Set_filter_in(LogLevel level) { filter_in = level; }

void FileSink::Set_filter_out(LogLevel level) { filter_out = level; }

void FileSink::Set_max_buffer(uint size) { max_buffer = size; }

void FileSink::Set_filename(std::string filename) { log_filename = filename; }

uint FileSink::Get_count() { return logcount.load(); }

std::string FileSink::Get_filename() { return log_filename; }

FileSink::~FileSink() {
    Save_to_file();
    std::cout << "Total save " << logcount << " log message to file\n";
}