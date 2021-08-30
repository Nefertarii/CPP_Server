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
    return socketfd;
}

int SERV::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        Errorlog("Bind error.", errno);
        return -1;
    }
    return 0;
}

int SERV::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        Errorlog("Listen error.", errno);
        return -1;
    }
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
        Errorlog("Get accept from", 0);
        Errorlog(inet_ntoa(cliaddr.sin_addr), 0);
        return 0;
    }
    return -1;
}

int SERV::Close(int fd) {
    if (close(fd) < 0) {
        Errorlog("Close error.", errno);
        return -1;
    }
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
        return 1; // normal close
    }
    else {
        *str = readbuf_tmp;
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
    return 0;
}

int SERV::Write(int socketfd, std::string str) {
    if(str.length() == 0) {
        return 1;
    }
    const char *tmpstr = str.c_str();
    int count = 0; //EINTR rewrite count
    while(count != REWRITEMAX) {
        if (write(socketfd, tmpstr, strlen(tmpstr)) < 0) {
            if (errno == EINTR) {
                if (count == 0) {
                    Warninglog("Signal interuption");
                }
                count++;
                continue;  
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                Warninglog("kernel cache full");
                return 1;
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
    return 0;
}

int SERV::Writefile(int socketfd, int filefd, off_t offset) {
    int count = 0;
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
    return 0;
}
