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

void Socket_Control_Server::SetConfig(Socket_Config* config) {
    if (config != nullptr) {
        socket_config = config;
    } else {
        std::cout << "\nConfig error!\n";
    }
}

void Socket_Control_Server::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p != nullptr) {
        this_log = log_p;
        have_upper = true;
    } else {
        this_log = new Log("Socket_Control_Log.txt", buffer_size);
        have_upper = false;
    }
}

int Socket_Control_Server::SocketListen() {
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(socket_config->port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    listenfd = Gsocket::Socket(AF_INET, SOCK_STREAM, 0, this_log);
    if (listenfd < 0) { return -1; }
    if (socket_config->reuseaddr) {
        if (Gsocket::ReuseAddress(listenfd, &socket_config->reuseaddr, this_log) < 0) {
            return -1;
        }
    }
    if (socket_config->reuseport) {
        if (Gsocket::ReusePort(listenfd, &socket_config->reuseport, this_log) < 0) {
            return -1;
        }
    }
    if (Gsocket::Bind(listenfd, (struct sockaddr*)&server_address,
                      sizeof(server_address), this_log) < 0) {
        return -1;
    }
    if (Gsocket::Listen(listenfd, 0, this_log) < 0) { return -1; }
    signal(SIGPIPE, SIG_IGN);
    return listenfd;
}

int Socket_Control_Server::SocketAccept() {
    if (socket_config->connect_nums == socket_config->connect_max) {
        return -1;
    } else {
        return Gsocket::Accept(listenfd, this_log);
    }
}

int Socket_Control_Server::SocketRead(int socketfd, std::string* readbuf) {
    return Gsocket::Read(socketfd, readbuf, socket_config->read_max, this_log);
}

int Socket_Control_Server::SocketWrite(int socketfd, std::string* message) {
    int ret = 0;
    size_t writemax = socket_config->write_max;
    if (message->size() > writemax) {
        std::string tmp(*message, 0, writemax);
        *message = message->substr(writemax, (message->size() - writemax));
        ret = Gsocket::Write(socketfd, &tmp, this_log);
    } else {
        ret = Gsocket::Write(socketfd, message, this_log);
        message->clear();
    }
    return ret;
}

int Socket_Control_Server::SocketDisconnect(int socketfd) {
    return Gsocket::Close(socketfd, this_log);
}

Socket_Control_Server::~Socket_Control_Server() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}

#endif