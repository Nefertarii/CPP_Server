#include "Glog.h"

int Savelog(LOGLEVEL level, const char *logstring) {
    std::string strlevel = Loglevel_map[level % LOGEND];
    std::string log = logstring;
    log = strlevel + " " + log + " " + Timer::Nowtime_str();
#ifdef DEBUG
    std::cout << Loglevel_map << "\n";
#else
    if (logindex + 1 == MAXLOG) {
        LOG[logindex] = log;
        logindex = 0;
        Savetofile(LOG);
    }
    else {
        LOG[logindex] = log;
        logindex += 1;
    }
#endif
    return 0;
}

int Savelog(LOGLEVEL level, std::string log) {
    return Savelog(level, string_to_char(log));
}

int Savetofile(std::string str) {
    std::fstream file;
    file.open("Log/log.txt", std::ios::in | std::ios::app);
    if (file) {
        file.write("\n", 1);
        std::string tmp = str + "\n";
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
            if(!logvec[i].empty()) {
                std::string tmp = logvec[i] + "\n";
                file.write(string_to_char(tmp), tmp.length());
            }

        }
        file.close();
        return 0;
    }
    else {
        file.close();
        return -1;
    }
}