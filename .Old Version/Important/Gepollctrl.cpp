#include "Gepollctrl.h"

void Epoll_Control::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p != nullptr) {
        this_log = log_p;
        have_upper = true;
    } else {
        this_log = new Log("Epoll_Control_Log.txt", buffer_size);
        have_upper = false;
    }
}

void Epoll_Control::SetEpollfd(int epollfd_) {
    epollfd = epollfd_;
    std::string log = "Epoll set, fd:" + std::to_string(epollfd) + ".";
    this_log->Infolog(log);
}

void Epoll_Control::Epolldel(int socketfd) {
    if (epollfd >= 0) {
        std::string log = "Epoll del, fd:" + std::to_string(socketfd) + ".";
        this_log->Infolog(log);
        epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
    }
}

void Epoll_Control::Epolladd(int socketfd, void* epolldata) {
    if (epollfd >= 0) {
        std::string log = "epoll add, fd:" + std::to_string(socketfd) + ".";
        this_log->Infolog(log);
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
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}