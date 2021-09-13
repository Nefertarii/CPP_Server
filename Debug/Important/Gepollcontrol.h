#ifndef GEPOLLCONTROL_H_
#define GEPOLLCONTROL_H_

#include "Glog.h"
#include <sys/epoll.h>

class Epoll_Control {
private:
    int epollfd;
    Log epoll_log;
public:
    Epoll_Control(std::string logfile);
    Epoll_Control(const char* logfile);
    Epoll_Control(std::string logfile, int epollfd_);
    Epoll_Control(const char* logfile, int epollfd_);
    void Set_epollfd(int epollfd_);
    void Epolladd(int socketfd, void* epolldata);
    void Epolldel(int socketfd);
    void Epollread(int socketfd, void* epolldata);
    void Epollwrite(int socketfd, void* epolldata);
    const int Epollfd() { return epollfd; }
    ~Epoll_Control() {}
};












Epoll_Control::Epoll_Control(std::string logfile) {
    epoll_log.Set(logfile, 200);
    epollfd = -1;
}

Epoll_Control::Epoll_Control(const char* logfile) {
    std::string logfile_ = logfile;
    epoll_log.Set(logfile_, 200);
    epollfd = -1;
}

Epoll_Control::Epoll_Control(std::string logfile, int epollfd_) {
    epoll_log.Set(logfile, 200);
    epollfd_ = epollfd;
}

Epoll_Control::Epoll_Control(const char* logfile, int epollfd_) {
    std::string logfile_ = logfile;
    epoll_log.Set(logfile_, 200);
    epollfd = -1;
}

void Epoll_Control::Set_epollfd(int epollfd_) {
    std::string log = "Epoll set, fd:" + std::to_string(epollfd) + ".";
    epoll_log.Infolog(log);
    epollfd = epollfd_;
}

void Epoll_Control::Epolldel(int socketfd) {
    if (epollfd >= 0) {
        std::string log = "Epoll del, fd:" + std::to_string(socketfd) + ".";
        epoll_log.Infolog(log);
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

#endif