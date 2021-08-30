#ifndef GLOG_H_
#define GLOG_H_

#include "Gstring.h"
#include "Gtime.h"
#include <iostream>
#include <fstream>
#include <vector>

enum LOGLEVEL {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4,
    LEVELEND = (1<<30)
};

void Set_save_size(int tmpsize);
int Savelog(LOGLEVEL level, const char *logstring);
int Savelog(LOGLEVEL level, std::string log);
int Savelog(std::vector<std::string> logvec);
int Savetofile(std::string str);
int Savetofile(std::vector<std::string> logvec);
inline const char *Strlevel(LOGLEVEL level);
inline bool Ismaximum();
inline void Savetotemp(LOGLEVEL level, std::string log, int err);
void Debuglog(const char *log, int err = 0);
void Debuglog(std::string log, int err = 0);

void Infolog(const char *log, int err = 0);
void Infolog(std::string log, int err = 0);

void Warninglog(const char *log, int err = 0);
void Warninglog(std::string log, int err = 0);

void Errorlog(const char *log, int err = 0);
void Errorlog(std::string log, int err = 0);

void Fatalog(const char *log, int err = 0);
void Fatalog(std::string log, int err = 0);

#endif