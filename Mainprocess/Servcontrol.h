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
    Badrespone badrequest;
    Timer serverclock;
    int listenfd;
    size_t concurrent;
    size_t MAXCLIENT;
    size_t REWRITEMAX;
    Gthreadpool threadpool;
public:
    Servercontrol_epoll();
    void Server_start_Epollcontrol();
    void Connect_accept();
    void Connect_method_get(Clientinfo *client, std::string *readbuf);
    void Connect_method_post(Clientinfo *client, std::string *readbuf);
    void Connect_disconnect(Clientinfo *client);
    void Send_responehead(Clientinfo *client);
    void Send_responebody(Clientinfo *client);
    void Send_responefile(Clientinfo *client);
    void Server_stop();
    ~Servercontrol_epoll();
};

#endif