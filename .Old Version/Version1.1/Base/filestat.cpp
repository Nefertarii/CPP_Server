#include "filestat.h"

using namespace Wasi::Base;

FileStat::FileStat() :
    file_name(),
    filefd(0),
    file_offset(0),
    file_size(0) {}

void FileStat::Init() {
    file_name.clear();
    filefd      = 0;
    file_offset = 0;
    file_size   = 0;
}

long FileStat::Remaining() { return file_size - file_offset; }