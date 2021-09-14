#ifndef SERVFUNC_H_
#define SERVFUNC_H_

//headfile include
#include "Glog.h"
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdarg>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <dirent.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <vector>

//setting
//Globalvalue
//const size_t MAXLOG = 16;                            //Log buffer maximum
//const char *PROTOCOL = "HTTP";                       //server used protocol


struct Filestate {
    int filefd = 0;
    size_t filelength = 0;
    size_t offset = 0;
};


//using
//using ULL = unsigned long long;

namespace Gsocket {
    int Socket(int family, int type, int protocal, Log* log_p);
    int Bind(int fd, const struct sockaddr* sa, socklen_t salen, Log* log_p);
    int Listen(int fd, int backlog, Log* log_p);
    int Accept(int listenfd, Log* log_p);
    int Close(int fd, Log* log_p);
    int Read(int socketfd, std::string* str, size_t readmax, Log* log_p);
    int Readfile(std::string filename_, struct Filestate* filestat_, Log* log_p);
    int Write(int socketfd, std::string* str, Log* log_p);
    int Writefile(int socketfd, int filefd, off_t offset, size_t writemax ,Log* log_p);
};


//function

int Gsocket::Socket(int family, int type, int protocol, Log* log_p) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        log_p->Errorlog("Socket open error.", errno);
        return -1;
    }
    log_p->Infolog("Socket open ok");
    return socketfd;
}

int Gsocket::Bind(int fd, const struct sockaddr* sa, socklen_t salen, Log* log_p) {
    if (bind(fd, sa, salen) < 0) {
        log_p->Errorlog("Bind set error.", errno);
        return -1;
    }
    log_p->Infolog("Bind set ok");
    return 0;
}

int Gsocket::Listen(int fd, int backlog, Log* log_p) {
    if (listen(fd, backlog) < 0) {
        log_p->Errorlog("Listen set error.", errno);
        return -1;
    }
    log_p->Infolog("Listen set ok");
    return 0;
}

int Gsocket::Accept(int listenfd, Log* log_p) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int cnt = 0;
    while (cnt != 10) {
        int connectfd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
        if (connectfd < 0) {
            if (errno == EINTR)
                continue;
            log_p->Errorlog("Accept error.", errno);
            return -1;
        }
        std::string log = inet_ntoa(cliaddr.sin_addr);
        log = "Get accept from " + log;
        log_p->Infolog(log);
        return connectfd;
    }
    log_p->Errorlog("Accept error.");
    return -1;
}

int Gsocket::Close(int fd, Log* log_p) {
    std::string log = "Close " + std::to_string(fd);
    if (close(fd) < 0) {
        log += " error.";
        log_p->Errorlog(log, errno);
        return -1;
    }
    log += " ok.";
    log_p->Infolog(log);
    return 0;
}

int Gsocket::Read(int socketfd, std::string* str, size_t readmax, Log* log_p) {
    char readbuf_tmp[readmax] = { 0 };
    int readsize = read(socketfd, readbuf_tmp, readmax);
    if (readsize < 0) {
        if (errno == EINTR) {
            log_p->Errorlog("Read error, Single break");
        } else if (errno == EPIPE) {
            log_p->Errorlog("Read error, Client close");
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            log_p->Errorlog("Read error", errno);
        }
        //error close
        return -1;
    } else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close";
        log_p->Infolog(log);
        return 1; // normal close
    } else {
        std::string log = "Read from " + std::to_string(socketfd);
        log_p->Infolog(log);
        *str = readbuf_tmp;
        return 0; // success
    }
}

int Gsocket::Readfile(std::string filename, struct Filestate* filestat_, Log* log_p) {
    struct stat file;
    int filefd = 0;
    
    const char* document = filename.c_str();
    std::cout << document << "\n";
    filefd = open(document, O_RDONLY);
    if (filefd < 0) {
        log_p->Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    if (stat(document, &file) < 0) {
        log_p->Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    filestat_->filefd = filefd;
    filestat_->filelength = file.st_size;
    filestat_->offset = 0;
    std::string log = "Readfile " + filename + " filefd: " + std::to_string(filestat_->filefd) + " success.";
    log_p->Infolog(log);
    return 0;
}

int Gsocket::Write(int socketfd, std::string* str, Log* log_p) {
    const char* tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    log_p->Infolog(log);
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            log_p->Errorlog("Write error", errno); //Signal interruption: slow system call
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            log_p->Warninglog("write error", errno); //kernel cache full
            return 2;
        } else {
            log_p->Errorlog("write error", errno);
            return -1;
        }
    }
    log += " success.";
    log_p->Infolog(log);
    return 0;
}

int Gsocket::Writefile(int socketfd, int filefd, off_t offset, size_t writemax ,Log* log_p) {
    if (sendfile(socketfd, filefd, &offset, writemax) < 0) {
        if (errno == EINTR) {
            log_p->Warninglog("Signal interuption");
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            log_p->Warninglog("kernel cache full");
            return 2;
        } else {
            log_p->Errorlog("Write error", errno);
            return -1;
        }
    }
    if (offset == 0) {
        std::string log = "Write filefd " + std::to_string(filefd) +
            " to: " + std::to_string(socketfd) + " success.";
        log_p->Infolog(log);
    }
    return 0;
}








/* 

//class type
class Socket_Func {
private:
    Log* socket_func_log;
    bool have_upper;                            //have upper levels log pointer;
    size_t READMAX;                             //Once read max length
    size_t WRITEMAX;                            //Once write max length
    size_t REWRITEMAX;                          //Error retry max times
    size_t LISTENPORT;                          //listen port
    size_t SINGLECLIENTS;                       //once thread max client
    std::string FILEDIR;                        //Default location for read file
    //const size_t MAXLOG = 16;                 //Log buffer maximum
    //const char *PROTOCOL = "HTTP";            //server used protocol
    void Init();
public:
    Socket_Func() { Init(); }
    void Setlog(Log* upper);
    void SET_READMAX(size_t readmax) { READMAX = readmax; }
    void SET_WRITEMAX(size_t writemax) { WRITEMAX = writemax; }
    void SET_REWRITEMAX(size_t rewritemax) { REWRITEMAX = rewritemax; }
    void SET_LISTENPORT(size_t listenport) { LISTENPORT = listenport; }
    void SET_SINGLECLIENTS(size_t single_core_client) { SINGLECLIENTS = single_core_client; }
    void SET_FILEDIR(std::string filedir) { FILEDIR = filedir; }
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr* sa, socklen_t salen);
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    int Read(int socketfd, std::string* str);
    int Readfile(std::string filename_, struct Filestate* filestat_);
    int Write(int socketfd, std::string* str);
    int Writefile(int socketfd, int filefd, off_t offset);
    ~Socket_Func();
};

void Socket_Func::Init() {
    size_t READMAX = 1024 * 4;
    size_t WRITEMAX = 1024 * 40;
    size_t REWRITEMAX = 5;
    size_t LISTENPORT = 8000;
    size_t SINGLECLIENTS = 8;
    std::string FILEDIR = "/home/cs18/vscode/Webserver/Blog/";
}

void Socket_Func::Setlog(Log* upper) {
    if (upper != nullptr) {
        socket_func_log = upper;
        have_upper = true;
    } else {
        socket_func_log = new Log(logfile, 200);
        have_upper = false;
    }
}

int Socket_Func::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        socket_func_log->Errorlog("Socket open error.", errno);
        return -1;
    }
    socket_func_log->Infolog("Socket open ok");
    return socketfd;
}

int Socket_Func::Bind(int fd, const struct sockaddr* sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        socket_func_log->Errorlog("Bind set error.", errno);
        return -1;
    }
    socket_func_log->Infolog("Bind set ok");
    return 0;
}

int Socket_Func::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        socket_func_log->Errorlog("Listen set error.", errno);
        return -1;
    }
    socket_func_log->Infolog("Listen set ok");
    return 0;
}

int Socket_Func::Accept(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int cnt = 0;
    while (cnt != 10) {
        int connectfd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
        if (connectfd < 0) {
            if (errno == EINTR)
                continue;
            socket_func_log->Errorlog("Accept error.", errno);
            return -1;
        }
        std::string log = inet_ntoa(cliaddr.sin_addr);
        log = "Get accept from " + log;
        socket_func_log->Infolog(log);
        return connectfd;
    }
    socket_func_log->Errorlog("Accept error.");
    return -1;
}

int Socket_Func::Close(int fd) {
    std::string log = "Close " + std::to_string(fd);
    if (close(fd) < 0) {
        log += " error.";
        socket_func_log->Errorlog(log, errno);
        return -1;
    }
    log += " ok.";
    socket_func_log->Infolog(log);
    return 0;
}

int Socket_Func::Read(int socketfd, std::string* str) {
    char readbuf_tmp[READMAX] = { 0 };
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            socket_func_log->Errorlog("Read error, Single break");
        } else if (errno == EPIPE) {
            socket_func_log->Errorlog("Read error, Client close");
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            socket_func_log->Errorlog("Read error", errno);
        }
        //error close
        return -1;
    } else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close";
        socket_func_log->Infolog(log);
        return 1; // normal close
    } else {
        std::string log = "Read from " + std::to_string(socketfd);
        socket_func_log->Infolog(log);
        *str = readbuf_tmp;
        return 0; // success
    }
}

int Socket_Func::Readfile(std::string filename_, struct Filestate* filestat_) {
    struct stat file;
    int filefd = 0;
    filename_ = FILEDIR + filename_;
    const char* filename = filename_.c_str();
    filefd = open(filename, O_RDONLY);
    if (filefd < 0) {
        socket_func_log->Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    if (stat(filename, &file) < 0) {
        socket_func_log->Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    filestat_->filefd = filefd;
    filestat_->filelength = file.st_size;
    filestat_->offset = 0;
    std::string log = "Readfile " + filename_ + " filefd: " + std::to_string(filestat_->filefd) + " success.";
    socket_func_log->Infolog(log);
    return 0;
}

int Socket_Func::Write(int socketfd, std::string* str) {
    const char* tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    socket_func_log->Infolog(log);
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            socket_func_log->Errorlog("Write error", errno); //Signal interruption: slow system call
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            socket_func_log->Warninglog("write error", errno); //kernel cache full
            return 2;
        } else {
            socket_func_log->Errorlog("write error", errno);
            return -1;
        }
    }
    log += " success.";
    socket_func_log->Infolog(log);
    return 0;
}

int Socket_Func::Writefile(int socketfd, int filefd, off_t offset) {
    if (sendfile(socketfd, filefd, &offset, WRITEMAX) < 0) {
        if (errno == EINTR) {
            socket_func_log->Warninglog("Signal interuption");
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            socket_func_log->Warninglog("kernel cache full");
            return 2;
        } else {
            socket_func_log->Errorlog("Write error", errno);
            return -1;
        }
    }
    if (offset == 0) {
        std::string log = "Write filefd " + std::to_string(filefd) +
            " to: " + std::to_string(socketfd) + " success.";
        socket_func_log->Infolog(log);
    }
    return 0;
}

Socket_Func::~Socket_Func() {
    if (!have_upper) {
        delete socket_func_log;
    }
}




*/













#endif