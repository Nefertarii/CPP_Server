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
    int Connect(int socketfd, const struct sockaddr* sa, socklen_t salen, Log* log_p);
    int Read(int socketfd, std::string* str, size_t readmax, Log* log_p);
    int Readfile(std::string filename_, struct Filestate* filestat_, Log* log_p);
    int Write(int socketfd, std::string* str, Log* log_p);
    int Writefile(int socketfd, int filefd, off_t offset, size_t writemax ,Log* log_p);
    int ReuseAddress(int socketfd, int *flag, Log* log_p);
    int ReusePort(int socketfd, int* flag, Log* log_p);
    void GetAddress(int socketfd, std::string* ip, std::string* port, Log* log_p);
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
    std::string log = "Listen set, fd:" + std::to_string(fd) + ".";
    log_p->Infolog(log);
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

int Gsocket::Connect(int socketfd, const struct sockaddr* sa, socklen_t salen, Log* log_p) {
    std::string log = "Connect " + std::to_string(socketfd);
    if (connect(socketfd, sa, salen) < 0) {
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

int Gsocket::ReuseAddress(int socketfd, int *flag, Log* log_p) {
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, flag, sizeof(int)) < 0) {
        log_p->Warninglog("Set reuse address fail.", errno);
        return -1;
    } else {
        log_p->Infolog("Set reuse address ok.");
        return 0;
    }
}

int Gsocket::ReusePort(int socketfd, int *flag, Log* log_p) {
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, flag, sizeof(int)) < 0) {
        log_p->Warninglog("Set reuse port fail.", errno);
        return -1;
    } else {
        log_p->Infolog("Set reuse port ok.");
        return 0;
    }
}

void Gsocket::GetAddress(int socketfd, std::string* ip, std::string* port, Log* log_p) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(socketfd, (struct sockaddr *)&client_address, &address_length);
    *port = std::to_string(ntohs(client_address.sin_port));
    *ip = inet_ntoa(client_address.sin_addr);
    std::string log = "Client IP:" + *ip + ":" + *port;
    log_p->Infolog(log);
}

#endif