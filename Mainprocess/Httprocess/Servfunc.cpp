#include "Servfunc.h"

const size_t READMAX = 1024 * 4;
const size_t WRITEMAX = 1024 * 4; 
const size_t REWRITEMAX = 4; 

int SERV::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        Errorlog("Socket error.", errno);
        return -1;
    }
    Infolog("Socket ok");
    return socketfd;
}

int SERV::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        Errorlog("Bind error.", errno);
        return -1;
    }
    Infolog("Bind ok");
    return 0;
}

int SERV::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        Errorlog("Listen error.", errno);
        return -1;
    }
    Infolog("Listen ok");
    return 0;
}

int SERV::Accept(int listenfd) {
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

int SERV::Close(int fd) {
    if (close(fd) < 0) {
        Errorlog("Close error.", errno);
        return -1;
    }
    std::string log = "Close " + std::to_string(fd) + " ok.";
    Infolog(log);
    return 0;
}

int SERV::Read(int socketfd, std::string *str) {
    char readbuf_tmp[READMAX] = {0};
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            Errorlog("Read error, Single break");
        }
        else if(errno == EPIPE) {
            Errorlog("Read error, Client close");
        }
        else if(errno != EAGAIN && errno != EWOULDBLOCK) {
            Errorlog("Read error", errno);
        }
        return -1; // error close
    }
    else if (readsize == 0) {
        std::string log = "client: " + std::to_string(socketfd) + " send close";
        Infolog(log);
        return 1; // normal close
    }
    else {
        std::string log = "Read from " + std::to_string(socketfd);
        Infolog(log);
        *str = readbuf_tmp;
        /*
        #ifdef DEBUG
            Infolog(*str);
        #else
            ;
        #endif
        */
        return 0;
    }
}

int SERV::Readfile(std::string filename_,struct Filestate *filestat_) {
    struct stat file;
    int filefd = 0;
    const char *filename = filename_.c_str();
    filefd = open(filename, O_RDONLY);
    if(filefd < 0) {
        Errorlog("Readfile error", errno);
        return 1; //local fail
    }
    if(stat(filename, &file) < 0) {
        Errorlog("Readfile error", errno);
        return 1; //local fail
    }
    filestat_->filefd = filefd;
    filestat_->filelength = file.st_size;
    filestat_->offset = 0;
    return 0;
}

int SERV::Write(int socketfd, std::string *str) {
    const char *tmpstr = str->c_str();
    size_t count = 0; //EINTR rewrite count
    std::string log = "Write: " + std::to_string(socketfd);
    Infolog(log);
    while (count != REWRITEMAX)
    {
        if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
            if (errno == EINTR) {
                if (count == 0) {
                    Warninglog("Write error", errno); //Signal interruption: slow system call
                }
                count++;
                continue;  
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                Warninglog("write error", errno); //kernel cache full
                return 2;
            }
            else {
                Errorlog("write error", errno);
                return -1;
            }
        }
    }
    if (count >= REWRITEMAX) {
        Warninglog("Write fail, Maximum number of rewrite");
        return 1;
    }
    log += " success.";
    Infolog(log);
    return 0;
}

int SERV::Writefile(int socketfd, int filefd, off_t offset) {
    int count = 0;
    std::string log = "Write file: " + std::to_string(socketfd);
    Infolog(log);
    while(count != REWRITEMAX) {
        if(sendfile(socketfd, filefd, &offset, WRITEMAX) < 0) {
            if (errno == EINTR) {
                if (count == 0) {
                    Warninglog("Signal interuption");
                }
                count++;
                continue;  
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                Warninglog("kernel cache full");
                return 1;
            }
            else {
                Errorlog("Write error", errno);
                return -1;
            }
        }
    }
    if (count >= REWRITEMAX) {
        Warninglog("Write file fail, Maximum number of rewrite");
        return 1;
    }
    log += " success.";
    Infolog(log);
    return 0;
}
