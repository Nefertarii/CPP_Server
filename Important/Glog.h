#ifndef GLOG_H_
#define GLOG_H_

#include "Gstring.h"
#include "Gtime.h"
#include <iostream>
#include <fstream>
#include <vector>

enum LOGLEVEL {
    INFO = 0,
    WARNING,
    ERROR,
    FATAL,
    LEVELEND = (1 << 30)
};

class Log {
private:
    std::string logfile;
    size_t MAXLOG;
    size_t logindex = 0;
    std::vector<std::string> tmplog_vec;
    inline bool Ismaximum();
    inline const char* Strlevel(LOGLEVEL level);
    void Savetotemp(LOGLEVEL level, std::string log, int err);
public:
    Log() {};
    Log(std::string filedir, size_t MAXLOG_);
    void Set(std::string filedir, size_t MAXLOG_);
    int Savelog(LOGLEVEL level, const char* logstring);
    int Savelog(LOGLEVEL level, std::string log);
    int Savelog(std::vector<std::string> logvec);
    int Savetofile(std::string str);
    int Savetofile(std::vector<std::string> logvec);
    int Savetofile();
    void Infolog(const char* log, int err = 0);
    void Infolog(std::string log, int err = 0);
    void Warninglog(const char* log, int err = 0);
    void Warninglog(std::string log, int err = 0);
    void Errorlog(const char* log, int err = 0);
    void Errorlog(std::string log, int err = 0);
    void Fatalog(const char* log, int err = 0);
    void Fatalog(std::string log, int err = 0);
    ~Log();
};

#endif