#include "Glog.h"

const size_t MAXLOG = 200;
int logindex = 0;
std::vector<std::string> tmplog_vec(MAXLOG, "");

static const char *Loglevel_map[] = {
    [DEBUG] = "DEBUG:",
    [INFO] = "INFO:",
    [WARNING] = "WARNING:",
    [ERROR] = "ERROR:",
    [FATAL] = "FATAL:"
};

void Set_save_size(int tmpsize) { tmplog_vec.resize(tmpsize); }

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

inline const char *Strlevel(LOGLEVEL level) { return Loglevel_map[level % LEVELEND]; }

inline bool Ismaximum() {
    if(logindex + 1  == MAXLOG)
        return true;
    return false;
}

inline void Savetotemp(LOGLEVEL level, const char *log, int err) {
#ifdef DEBUG
    std::cout << log << "\n";
#else
    std::string strlevel = Strlevel(level);
    std::string strlog = log;
    if(!err) {
        tmplog_vec[logindex] = strlevel + " " + strlog;
        return;
    }
    const char *tmperr = strerror(err);
    std::string strerr =tmperr;
    tmplog_vec[logindex] = strlevel + " " + strlog + strerr;
#endif

}

void Debuglog(const char *log, int err) {
    if(!Ismaximum()) {
        Savetotemp(DEBUG, log, err);
        logindex++;
        return;
    }
    Savetotemp(DEBUG, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Debuglog(std::string log, int err) { Debuglog(string_to_char(log), err); }

void Infolog(const char *log, int err) {
    if(!Ismaximum()) {
        Savetotemp(INFO, log, err);
        logindex++;
        return;
    }
    Savetotemp(INFO, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Infolog(std::string log, int err) { Infolog(string_to_char(log), err); }

void Warninglog(const char *log, int err) {
    if(!Ismaximum()) {
        Savetotemp(WARNING, log, err);
        logindex++;
        return;
    }
    Savetotemp(WARNING, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Warninglog(std::string log, int err) { Warninglog(string_to_char(log), err); }

void Errorlog(const char *log, int err) {
    if(!Ismaximum()) {
        Savetotemp(ERROR, log, err);
        logindex++;
        return;
    }
    Savetotemp(ERROR, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Errorlog(std::string log, int err) { Errorlog(string_to_char(log), err); }

void Fatalog(const char *log, int err) {
    if(!Ismaximum()) {
        Savetotemp(FATAL, log, err);
        logindex++;
        return;
    }
    Savetotemp(FATAL, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Fatalog(std::string log, int err) { Fatalog(string_to_char(log), err); }