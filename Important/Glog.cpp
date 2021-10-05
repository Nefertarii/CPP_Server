#include "Glog.h"

static const char* Loglevel_map[] = {
    [INFO] = "INFO:",
    [WARNING] = "WARNING:",
    [ERROR] = "ERROR:",
    [FATAL] = "FATAL:"
};

inline const char* Log::Strlevel(LOGLEVEL level) { return Loglevel_map[level % LEVELEND]; }

inline bool Log::Ismaximum() {
    if (logindex + 1 == MAXLOG)
        return true;
    return false;
}

void Log::Savetotemp(LOGLEVEL level, std::string log, int err) {
    std::string strlevel = Strlevel(level);
    std::string strlog = log;
    std::string strtime = std::to_string(Timer::Nowtime_ms());
#if defined(DEBUG)
    if (!err) {
        strlog = strlevel + "" + strlog + "\t Data:" + strtime;
        std::cout << strlog << "\n";
        return;
    }
    const char* tmperr = strerror(err);
    std::string strerr = tmperr;
    strlog = strlevel + " " + strlog + " " + strerr + "\t Data:" + strtime;
    std::cout << strlog << "\n";
#elif defined(NOLOG)
    //do nothing
    ;
#else
    if (!err) {
        tmplog_vec[logindex] = strlevel + " " + strlog + "\t Data:" + strtime;
        return;
    }
    const char* tmperr = strerror(err);
    std::string strerr = tmperr;
    tmplog_vec[logindex] = strlevel + " " + strlog + " " + strerr + "\t Data:" + strtime;
#endif
}

Log::Log(std::string filedir, size_t MAXLOG_) {
    logfile = filedir;
    MAXLOG = MAXLOG_;
    tmplog_vec.resize(MAXLOG);
}

void Log::Set(std::string filedir, size_t MAXLOG_) {
    logfile = filedir;
    MAXLOG = MAXLOG_;
    tmplog_vec.resize(MAXLOG);
}

int Log::Savetofile(std::string str) {
    std::fstream file;
    file.open(logfile, std::ios::out | std::ios::app);
    if (file) {
        file.write("\n", 1);
        std::string tmp = str + "\n";
        file.write(string_to_char(tmp), tmp.length());
        file.close();
        return 0;
    } else {
        file.close();
        return -1;
    }
}

int Log::Savetofile(std::vector<std::string> logvec) {
    if (logvec.size() >= 1) {
        std::fstream file;
        logindex = 0;
        file.open(logfile, std::ios::in | std::ios::app);
        if (file) {
            std::cout << "logfile:" + logfile + " save...\n";
            file.write("\n", 1);
            int size = logvec.size();
            for (int i = 0; i != size; i++) {
                if (!logvec[i].empty()) {
                    std::string tmp = logvec[i] + "\n";
                    file.write(string_to_char(tmp), tmp.length());
                    logvec[i].clear();
                }
            }
            file.close();
            return 0;
        } else {
            file.close();
            std::ofstream newfile("log.txt");
            if (newfile) {
                std::cout << "log save newfile ...\n";
                newfile.write("\n", 1);
                int size = logvec.size();
                for (int i = 0; i != size; i++) {
                    if (!logvec[i].empty()) {
                        std::string tmp = logvec[i] + "\n";
                        newfile.write(string_to_char(tmp), tmp.length());
                        logvec[i].clear();
                    }
                }
                newfile.close();
                return 0;
            }
            std::cout << "log save fail!!!\n";
            return -1;
        }
    }
    return 0;
}

int Log::Savetofile() { return Savetofile(tmplog_vec); }

void Log::Infolog(const char* log, int err) {
    if (!Ismaximum()) {
        Savetotemp(INFO, log, err);
        logindex++;
        return;
    }
    Savetotemp(INFO, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Log::Infolog(std::string log, int err) { Infolog(string_to_char(log), err); }

void Log::Warninglog(const char* log, int err) {
    if (!Ismaximum()) {
        Savetotemp(WARNING, log, err);
        logindex++;
        return;
    }
    Savetotemp(WARNING, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Log::Warninglog(std::string log, int err) { Warninglog(string_to_char(log), err); }

void Log::Errorlog(const char* log, int err) {
    if (!Ismaximum()) {
        Savetotemp(ERROR, log, err);
        logindex++;
        return;
    }
    Savetotemp(ERROR, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Log::Errorlog(std::string log, int err) { Errorlog(string_to_char(log), err); }

void Log::Fatalog(const char* log, int err) {
    if (!Ismaximum()) {
        Savetotemp(FATAL, log, err);
        logindex++;
        return;
    }
    Savetotemp(FATAL, log, err);
    logindex = 0;
    Savetofile(tmplog_vec);
}

void Log::Fatalog(std::string log, int err) { Fatalog(string_to_char(log), err); }

Log::~Log() {
    Savetofile();
}