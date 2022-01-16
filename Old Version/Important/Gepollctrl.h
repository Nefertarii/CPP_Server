#ifndef GEPOLLCTRL_H_
#define GEPOLLCTRL_H_

#include "Glog.h"
#include <sys/epoll.h>

class Epoll_Control {
private:
    int epollfd;
    Log* this_log;
    bool have_upper;
public:
    Epoll_Control() { epollfd = -1; }
    void SetLog(Log* log_p, size_t buffer_size);
    void SetEpollfd(int epollfd_);
    void Epolladd(int socketfd, void* epolldata);
    void Epolldel(int socketfd);
    void Epollread(int socketfd, void* epolldata);
    void Epollwrite(int socketfd, void* epolldata);
    const int Epollfd() { return epollfd; }
    ~Epoll_Control();
};

#endif