#ifndef GLOG_H_
#define GLOG_H_

#include "Gstring.h"
#include "Gtime.h"
#include "iostream"
#include <fstream>
#include <vector>

extern const size_t MAXLOG;

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

static size_t logindex;
static std::vector<std::string> LOG(MAXLOG, "");

int Savelog(LOGLEVEL level, const char *logstring);
int Savelog(LOGLEVEL level, std::string log);
int Savetofile(std::string str);
int Savetofile(std::vector<std::string> logvec);

#endif