#ifndef GLOG_H_
#define GLOG_H_

#include "Gstring.h"
#include <fstream>
#include <vector>

extern const size_t MAXthread;

enum LOGLEVEL {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4,
    LOGEND = (1<<30)
};

static const char *Loglevel_map[] = {
    [DEBUG] = "DEBUG:",
    [INFO] = "INFO:",
    [WARNING] = "WARNING:",
    [ERROR] = "ERROR:",
    [FATAL] = "FATAL:"
};

//index 0~40 main thread
//index 40~40+40*thread id
static std::vector<std::string> LOG(MAXthread * 20, "");

int Savelog(LOGLEVEL level, const char *logstring, int index);
int Savetofile(std::string str);
int Savetofile(std::vector<std::string> logvec);

#endif