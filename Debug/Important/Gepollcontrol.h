#ifndef GEPOLLCONTROL_H_
#define GEPOLLCONTROL_H_

#include "Glog.h"
#include <sys/epoll.h>

class Epoll_Control {
private:
    int epollfd;
    Log* epoll_log;
    bool have_upper;
public:
    Epoll_Control() { epollfd = -1; }
    void SetLog(Log* upper, size_t buffer_size);
    void SetEpollfd(int epollfd_);
    void Epolladd(int socketfd, void* epolldata);
    void Epolldel(int socketfd);
    void Epollread(int socketfd, void* epolldata);
    void Epollwrite(int socketfd, void* epolldata);
    const int Epollfd() { return epollfd; }
    ~Epoll_Control();
};














void Epoll_Control::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        epoll_log = upper;
        have_upper = true;
    } else {
        epoll_log = new Log(logfile, buffer_size);
        have_upper = false;
    }
}

void Epoll_Control::Set_epollfd(int epollfd_) {
    std::string log = "Epoll set, fd:" + std::to_string(epollfd) + ".";
    epoll_log->Infolog(log);
    epollfd = epollfd_;
}

void Epoll_Control::Epolldel(int socketfd) {
    if (epollfd >= 0) {
        std::string log = "Epoll del, fd:" + std::to_string(socketfd) + ".";
        epoll_log->Infolog(log);
        epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
    }
}

void Epoll_Control::Epolladd(int socketfd, void* epolldata) {
    if (epollfd >= 0) {
        std::string log = "epoll add, fd:" + std::to_string(socketfd) + ".";
        Infolog(log);
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        ev.data.ptr = epolldata;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
    }
}

void Epoll_Control::Epollread(int socketfd, void* epolldata) {
    if (epollfd >= 0) {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        ev.data.ptr = epolldata;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    }
}

void Epoll_Control::Epollwrite(int socketfd, void* epolldata) {
    if (epollfd >= 0) {
        struct epoll_event ev;
        ev.events = EPOLLOUT | EPOLLET;
        ev.data.ptr = epolldata;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    }
}

Epoll_Control::~Epoll_Control() {
    if (!have_upper) {
        delete epoll_log;
    }
}

#endif