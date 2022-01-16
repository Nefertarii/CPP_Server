#ifndef GSOCKETCTRL_CLIENT_H_
#define GSOCKETCTRL_CLIENT_H_

#include "Gsocketfunc.h"

class Socket_Control_Client {
private:
    int serverfd;
    Log* this_log;
    bool have_upper;
    bool have_connect;
    Socket_Config* socket_config;
    std::string ip;
    int port;
    std::string token;

public:
    Socket_Control_Client() = default;
    void SetConfig(Socket_Config* config);
    void SetLog(Log* upper, size_t buffer_size);
    void SetConnect(std::string ip, int port);
    int SocketConnect();
    int SocketRead(std::string* readbuf);
    int SocketWrite(std::string* message);
    int SocketDisconnect();
    ~Socket_Control_Client();
};

#endif