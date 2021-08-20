#ifndef GLOG_H_
#define GLOG_H_

#include "Gstring.h"
#include "Gthread.h"
#include <fstream>
#include <vector>

extern const size_t MAXthread;

static std::vector<std::string> LOG(MAXthread * 20, "empty");

int Savelog(const char *logstring, int index);
int Savetofile(std::vector<std::string> logvec);


#endif