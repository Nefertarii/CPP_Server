#ifndef GSOCKETCTRL_SERVER_H_
#define GSOCKETCTRL_SERVER_H_

#include "Gsocketfunc.h"

class Socket_Control_Server {
private:
    Log* this_log;
    bool have_upper;        //have upper levels log pointer;
    int listenfd;
    Socket_Config* socket_config;

public:
    Socket_Control_Server() {}
    void SetConfig(Socket_Config* config);
    void SetLog(Log* log_p, size_t buffer_size);
    int SocketListen();
    int SocketAccept();
    int SocketRead(int socketfd, std::string* str);      //read once;
    int SocketWrite(int socketfd, std::string* message); //write once;
    int SocketDisconnect(int socketfd);
    ~Socket_Control_Server();
};

#endif