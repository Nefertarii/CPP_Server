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

void Socket_Control_Client::SetConfig(Socket_Config* config) {
    if (config != nullptr) {
        socket_config = config;
    } else {
        std::cout << "\nConfig error!\n";
    }
}

void Socket_Control_Client::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        this_log = upper;
        have_upper = true;
    } else {
        this_log = new Log("Socket_Control_Log.txt", buffer_size);
        have_upper = false;
    }
}

void Socket_Control_Client::SetConnect(std::string ip_, int port_) {
    ip = ip_;
    port = port_;
}

int Socket_Control_Client::SocketConnect() {
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    serverfd = Gsocket::Socket(AF_INET, SOCK_STREAM, 0, this_log);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
    Gsocket::Connect(serverfd, (sockaddr*)&servaddr, sizeof(servaddr), this_log);
    return 0;
}

int Socket_Control_Client::SocketRead(std::string* readbuf) {
    return Gsocket::Read(serverfd, readbuf, socket_config->read_max, this_log);
}

int Socket_Control_Client::SocketWrite(std::string* message) {
    int ret = 0;
    size_t writemax = socket_config->write_max;
    if (message->size() > writemax) {
        std::string tmp(*message, 0, writemax);
        *message = message->substr(writemax, (message->size() - writemax));
        ret = Gsocket::Write(serverfd, &tmp, this_log);
    } else {
        ret = Gsocket::Write(serverfd, message, this_log);
        message->clear();
    }
    return ret;
}

int Socket_Control_Client::SocketDisconnect() {
    return Gsocket::Close(serverfd, this_log);
}

Socket_Control_Client::~Socket_Control_Client() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}







#endif