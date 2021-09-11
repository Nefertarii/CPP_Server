#include "Servfunc.h"

int Servfunc::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        Errorlog("Socket open error.", errno);
        return -1;
    }
    Infolog("Socket open ok");
    return socketfd;
}

int Servfunc::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
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
        int connectfd = accept(listenfd, (sockaddr *)&cliaddr, &cliaddrlen);
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

int Servfunc::Read(int socketfd, std::string *str) {
    char readbuf_tmp[READMAX] = {0};
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            Errorlog("Read error, Single break");
        } else if(errno == EPIPE) {
            Errorlog("Read error, Client close");
        } else if(errno != EAGAIN && errno != EWOULDBLOCK) {
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

int Servfunc::Readfile(std::string filename_,struct Filestate *filestat_) {
    struct stat file;
    int filefd = 0;
    filename_ = FILEDIR + filename_;
    const char *filename = filename_.c_str();
    filefd = open(filename, O_RDONLY);
    if(filefd < 0) {
        Errorlog("Readfile error", errno);
        return -1; //local fail
    }
    if(stat(filename, &file) < 0) {
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

int Servfunc::Write(int socketfd, std::string *str) {
    const char *tmpstr = str->c_str();
    std::string log = "Write to: " + std::to_string(socketfd);
    Infolog(log);
    if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
        if (errno == EINTR) {
            Errorlog("Write error", errno); //Signal interruption: slow system call
            return 1;
        } else if(errno == EAGAIN || errno == EWOULDBLOCK) {
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