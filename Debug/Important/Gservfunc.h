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
static const size_t READMAX = 1024 * 40;                            //Once read max length
static const size_t WRITEMAX = 1024 * 40;                           //Once write max length
static const size_t REWRITEMAX = 5;                                 //Error retry max times
static const size_t LISTENPORT = 8000;                                //listen port
static const size_t SINGLECLIENTS = 8;                              //once thread max client
static const std::string FILEDIR = "/home/cs18/vscode/Webserver/Blog/";   //Default location for read file
//const size_t MAXLOG = 16;                            //Log buffer maximum
//const char *PROTOCOL = "HTTP";                       //server used protocol

struct Filestate {
    int filefd = 0;
    uint filelength = 0;
    uint offset = 0;
};

//using
using ULL = unsigned long long;

//System function rewrite
namespace Servfunc {
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr* sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    int Read(int socketfd, std::string* str);
    int Readfile(std::string filename_, struct Filestate* filestat_);
    int Write(int socketfd, std::string* str);
    int Writefile(int socketfd, int filefd, off_t offset);
};

int Servfunc::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        Errorlog("Socket open error.", errno);
        return -1;
    }
    Infolog("Socket open ok");
    return socketfd;
}

int Servfunc::Bind(int fd, const struct sockaddr* sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        Errorlog("Bind set error.", errno);
        return -1;
    }
    Infolog("Bind set ok");
    return 0;
}

int Servfunc::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        Errorlog("Listen set error.", errno);
        return -1;
    }
    Infolog("Listen set ok");
    return 0;
}

int Servfunc::Accept(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int cnt = 0;
    while (cnt != 10) {
        int connectfd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
        if (connectfd < 0) {
            if (errno == EINTR)
                continue;
            Errorlog("Accept error.", errno);
            return -1;
        }
        std::string log = inet_ntoa(cliaddr.sin_addr);
        log = "Get accept from " + log;
        Infolog(log);
        return connectfd;
    }
    Errorlog("Accept error.");
    return -1;
}

int Servfunc::Close(int fd) {
    std::string log = "Close " + std::to_string(fd);
    if (close(fd) < 0) {
        log += " error.";
        Errorlog(log, errno);
        return -1;
    }
    log += " ok.";
    Infolog(log);
    return 0;
}

int Servfunc::Read(int socketfd, std::string* str) {
    char readbuf_tmp[READMAX] = { 0 };
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            Errorlog("Read error, Single break");
        } else if (errno == EPIPE) {
            Errorlog("Read error, Client close");
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            Errorlog("Read error", errno);
        }
        //error close
        return -1;
    } else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close";
        Infolog(log);
        return 1; // normal close
    } else {
        std::string log = "Read from " + std::to_string(socketfd);
        Infolog(log);
        *str = readbuf_tmp;
        return 0; // success
    }
}

int Servfunc::Readfile(std::string filename_, struct Filestate* filestat_) {
    struct stat file;
    int filefd = 0;
    filename_ = FILEDIR + filename_;
    const char* filename = filename_.c_str();
    filefd = open(filename, O_RDONLY);
    if (filefd < 0) {
        Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    if (stat(filename, &file) < 0) {
        Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    filestat_->filefd = filefd;
    filestat_->filelength = file.st_size;
    filestat_->offset = 0;
    std::string log = "Readfile " + filename_ + " filefd: " + std::to_string(filestat_->filefd) + " success.";
    Infolog(log);
    return 0;
}

int Servfunc::Write(int socketfd, std::string* str) {
    const char* tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    Infolog(log);
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            Errorlog("Write error", errno); //Signal interruption: slow system call
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            Warninglog("write error", errno); //kernel cache full
            return 2;
        } else {
            Errorlog("write error", errno);
            return -1;
        }
    }
    log += " success.";
    Infolog(log);
    return 0;
}

int Servfunc::Writefile(int socketfd, int filefd, off_t offset) {
    if (sendfile(socketfd, filefd, &offset, WRITEMAX) < 0) {
        if (errno == EINTR) {
            Warninglog("Signal interuption");
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            Warninglog("kernel cache full");
            return 2;
        } else {
            Errorlog("Write error", errno);
            return -1;
        }
    }
    if (offset == 0) {
        std::string log = "Write filefd " + std::to_string(filefd) +
            " to: " + std::to_string(socketfd) + " success.";
        Infolog(log);
    }
    return 0;
}



//class type
class HTTP_Func {
private:
    Log HTTP_Func_log;
    size_t READMAX;                             //Once read max length
    size_t WRITEMAX;                            //Once write max length
    size_t REWRITEMAX;                          //Error retry max times
    size_t LISTENPORT;                          //listen port
    size_t SINGLECLIENTS;                       //once thread max client
    std::string FILEDIR;                        //Default location for read file
    //const size_t MAXLOG = 16;                 //Log buffer maximum
    //const char *PROTOCOL = "HTTP";            //server used protocol
public:
    HTTP_Func();
    void SET_READMAX(size_t readmax) { READMAX = readmax; }
    void SET_WRITEMAX(size_t writemax) { WRITEMAX=writemax; }
    void SET_REWRITEMAX(size_t rewritemax)  { REWRITEMAX=rewritemax; }
    void SET_LISTENPORT(size_t listenport)  { LISTENPORT=listenport; }
    void SET_SINGLECLIENTS(size_t single_core_client) { SINGLECLIENTS = single_core_client; }
    void SET_FILEDIR(std::string filedir) { FILEDIR = filedir; }
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr* sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    int Read(int socketfd, std::string* str);
    int Readfile(std::string filename_, struct Filestate* filestat_);
    int Write(int socketfd, std::string* str);
    int Writefile(int socketfd, int filefd, off_t offset);
    ~HTTP_Func();
};

HTTP_Func::HTTP_Func() {
    size_t READMAX = 1024 * 4;
    size_t WRITEMAX = 1024 * 40;
    size_t REWRITEMAX = 5;
    size_t LISTENPORT = 8000;
    size_t SINGLECLIENTS = 8;
    std::string FILEDIR = "/home/cs18/vscode/Webserver/Blog/";
}

int HTTP_Func::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        HTTP_Func_log.Errorlog("Socket open error.", errno);
        return -1;
    }
    HTTP_Func_log.Infolog("Socket open ok");
    return socketfd;
}

int HTTP_Func::Bind(int fd, const struct sockaddr* sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        HTTP_Func_log.Errorlog("Bind set error.", errno);
        return -1;
    }
    HTTP_Func_log.Infolog("Bind set ok");
    return 0;
}

int HTTP_Func::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        HTTP_Func_log.Errorlog("Listen set error.", errno);
        return -1;
    }
    HTTP_Func_log.Infolog("Listen set ok");
    return 0;
}

int HTTP_Func::Accept(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int cnt = 0;
    while (cnt != 10) {
        int connectfd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
        if (connectfd < 0) {
            if (errno == EINTR)
                continue;
            HTTP_Func_log.Errorlog("Accept error.", errno);
            return -1;
        }
        std::string log = inet_ntoa(cliaddr.sin_addr);
        log = "Get accept from " + log;
        HTTP_Func_log.Infolog(log);
        return connectfd;
    }
    HTTP_Func_log.Errorlog("Accept error.");
    return -1;
}

int HTTP_Func::Close(int fd) {
    std::string log = "Close " + std::to_string(fd);
    if (close(fd) < 0) {
        log += " error.";
        HTTP_Func_log.Errorlog(log, errno);
        return -1;
    }
    log += " ok.";
    HTTP_Func_log.Infolog(log);
    return 0;
}

int HTTP_Func::Read(int socketfd, std::string* str) {
    char readbuf_tmp[READMAX] = { 0 };
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            HTTP_Func_log.Errorlog("Read error, Single break");
        } else if (errno == EPIPE) {
            HTTP_Func_log.Errorlog("Read error, Client close");
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            HTTP_Func_log.Errorlog("Read error", errno);
        }
        //error close
        return -1;
    } else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close";
        HTTP_Func_log.Infolog(log);
        return 1; // normal close
    } else {
        std::string log = "Read from " + std::to_string(socketfd);
        HTTP_Func_log.Infolog(log);
        *str = readbuf_tmp;
        return 0; // success
    }
}

int HTTP_Func::Readfile(std::string filename_, struct Filestate* filestat_) {
    struct stat file;
    int filefd = 0;
    filename_ = FILEDIR + filename_;
    const char* filename = filename_.c_str();
    filefd = open(filename, O_RDONLY);
    if (filefd < 0) {
        HTTP_Func_log.Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    if (stat(filename, &file) < 0) {
        HTTP_Func_log.Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    filestat_->filefd = filefd;
    filestat_->filelength = file.st_size;
    filestat_->offset = 0;
    std::string log = "Readfile " + filename_ + " filefd: " + std::to_string(filestat_->filefd) + " success.";
    HTTP_Func_log.Infolog(log);
    return 0;
}

int HTTP_Func::Write(int socketfd, std::string* str) {
    const char* tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    HTTP_Func_log.Infolog(log);
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            HTTP_Func_log.Errorlog("Write error", errno); //Signal interruption: slow system call
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            HTTP_Func_log.Warninglog("write error", errno); //kernel cache full
            return 2;
        } else {
            HTTP_Func_log.Errorlog("write error", errno);
            return -1;
        }
    }
    log += " success.";
    HTTP_Func_log.Infolog(log);
    return 0;
}

int HTTP_Func::Writefile(int socketfd, int filefd, off_t offset) {
    if (sendfile(socketfd, filefd, &offset, WRITEMAX) < 0) {
        if (errno == EINTR) {
            HTTP_Func_log.Warninglog("Signal interuption");
            return 1;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            HTTP_Func_log.Warninglog("kernel cache full");
            return 2;
        } else {
            HTTP_Func_log.Errorlog("Write error", errno);
            return -1;
        }
    }
    if (offset == 0) {
        std::string log = "Write filefd " + std::to_string(filefd) +
            " to: " + std::to_string(socketfd) + " success.";
        HTTP_Func_log.Infolog(log);
    }
    return 0;
}

HTTP_Func::~HTTP_Func() { ; }


#endif