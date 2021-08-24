#include "Glog.h"

int Savelog(LOGLEVEL level, const char *logstring, int index) {
    std::string tmp = Loglevel_map[level % LOGEND];
    tmp += logstring + " " + Timer::Nowtime_str();
    if (index % index)
    {
        LOG[index] = tmp;
        return 0;
    }
    else {
        LOG[index] = tmp;
        if(Savetofile(LOG) < 0)
            return -1;
        return 1;
    }
}

int Savetofile(std::string str) {
    std::fstream file;
    file.open("Log/log.txt", std::ios::in | std::ios::app);
    if (file) {
        file.write("\n", 1);
        tmp = str + "\n";
        file.write(string_to_char(tmp), tmp.length());
        file.close();
        return 0;
    }
    else {
        file.close();
        return -1;
    }
}

int Savetofile(std::vector<std::string> logvec) {
    std::fstream file;
    file.open("Log/log.txt", std::ios::in | std::ios::app);
    if (file) {
        file.write("\n", 1);
        int size = logvec.size();
        for (int i = 0; i != size; i++) {
            if(!logvec[i].empty())
            std::string tmp = logvec[i];
            tmp += "\n";
            file.write(string_to_char(tmp), tmp.length());
        }
        file.close();
        return 0;
    }
    else {
        file.close();
        return -1;
    }
}