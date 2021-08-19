#ifndef LOG_H_
#define LOG_H_

#include "Gstring.h"
#include <fstream>
#include <vector>

static std::vector<std::string> LOG(200, "empty");

int Savelog(const char *logstring, int index) {
    std::string tmp = logstring;
    if(index % index) {
        LOG[index] = tmp;
        return 0;
    }
    else {
        LOG[index] = tmp;
        Savelog(LOG);
        return 1;
    }
}

int Savelog(std::vector<std::string> logvec) {
    std::fstream file;
    file.open("Log/log.txt", std::ios::in | std::ios::app);
    if (file) {
        file.write("\n", 1);
        int size = logvec.size();
        for (int i = 0; i != size; i++) {
            std::string tmp = logvec[i];
            tmp += "\n";
            file.write(sToc(tmp), tmp.length());
        }
        file.close();
        return 0;
    }
    else {
        file.close();
        return -1;
    }
}


#endif