#include "Headfile.h"

int SERV::Socket(int family, int type, int protocol) {
    int socketfd = socket(family, type, protocol);
    if (socketfd < 0) {
        SERV::Syserrlog("Socket error.", errno);
        return -1;
    }
    return socketfd;
}

int SERV::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        SERV::Syserrlog("Bind error.", errno);
        return -1;
    }
    return 0;
}

int SERV::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        SERV::Syserrlog("Listen error.", errno);
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
            SERV::Syserrlog("Accept error.", errno);
            return -1;
            }
        Syserrlog("Get accept from", 0);
        Syserrlog(inet_ntoa(cliaddr.sin_addr), 0);
        return 0;
    }
    return -1;
}

int SERV::Close(int fd) {
    if (close(fd) < 0) {
        SERV::Syserrlog("Close error.", errno);
        return -1;
    }
    return 0;
}

int SERV::Read(int socketfd, std::string *str) {
    char readbuf_tmp[READMAX] = {0};
    int readsize = read(socketfd, readbuf_tmp, READMAX);
    if (readsize < 0) {
        if (errno == EINTR) {
            Savelog(ERROR, "Read error, Single break");
        }
        else if(errno == EPIPE) {
            Savelog(ERROR, "Read error, Client close");
        }
        else if(errno != EAGAIN && errno != EWOULDBLOCK) {
            SERV::Syserrlog("Read error", errno);
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
        SERV::Syserrlog("Readfile error", errno);
        return 1; //local fail
    }
    if(stat(filename, &file) < 0) {
        SERV::Syserrlog("Readfile error", errno);
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
                    Savelog(WARNING, "Signal interuption");
                }
                count++;
                continue;  
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                Savelog(WARNING, "kernel cache full");
                return 1;
            }
            else {
                SERV::Syserrlog("write error", errno);
                return -1;
            }
        }
    }
    if (count >= REWRITEMAX) {
        Savelog(WARNING, "Write fail, Maximum number of rewrite");
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
                    Savelog(WARNING, "Signal interuption");
                }
                count++;
                continue;  
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                Savelog(WARNING, "kernel cache full");
                return 1;
            }
            else {
                SERV::Syserrlog("Write error", errno);
                return -1;
            }
        }
    }
    if (count >= REWRITEMAX) {
        Savelog(WARNING, "Write file fail, Maximum number of rewrite");
        return 1;
    }
    return 0;
}

void SERV::Syserrlog(const char *fmt, int err) {
    const char *tmperr = strerror(err);
    if(err) {
        Savelog(ERROR, fmt);
        Savelog(ERROR, tmperr);
    }
    else {
        Savelog(ERROR, fmt);
    }
}

ULL SERV::Pthreadid() {
    std::ostringstream buf;
    buf << std::this_thread::get_id();
    std::string stid = buf.str();
    ULL tid = std::stoull(stid);
    return tid;
}
