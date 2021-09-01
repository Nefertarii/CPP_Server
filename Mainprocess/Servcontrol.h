#ifndef SERVCONTROL_H_
#define SERVCONTROL_H_

#include "Protocol.h"
#include "Headfile.h"
#include "Servprocess.h"

class Servercontrol_epoll {
private:
    std::vector<Clientinfo> clients;
    Httpconnect connectctrl;
    Httprocess processctrl;
    Httprespone responectrl;
    Epollcontrol epollctrl;
    Timer serverclock;
    int listenfd;
    int concurrent;
    int MAXCLIENT;
    //Gthreadpool threadpool(MAXCLIENT);
public:
    Servercontrol_epoll();
    void Server_start_Epollcontrol();
    void Server_stop();
    ~Servercontrol_epoll();
};

#endif