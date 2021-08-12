#ifndef LOG_H_
#define LOG_H_

#include "Gstring.h"
#include <fstream>
#include <vector>

int saveLog(std::string logstr)
{
    std::fstream log;
    log.open("Log/log.txt", std::ios::in | std::ios::app);
    if (log)
    {
        log.write("\n", 1);
        logstr += "\n";
        log.write(sToc(logstr), logstr.length());
        log.close();
        return 0;
    }
    else
    {
        log.close();
        return -1;
    }
}

int saveLog(std::vector<std::string> logvec)
{
    std::fstream log;
    log.open("Log/log.txt", std::ios::in | std::ios::app);
    if (log)
    {
        log.write("\n", 1);
        int size = logvec.size();
        for (int i = 0; i != size; i++)
        {
            std::string tmp = logvec[i];
            tmp += "\n";
            log.write(sToc(tmp), tmp.length());
        }
        log.close();
        return 0;
    }
    else
    {
        log.close();
        return -1;
    }
}


#endif