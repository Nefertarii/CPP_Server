#ifndef GSOCKETCTRL_H_
#define GSOCKETCTRL_H_

#include "Glog.h"
#include "Gsocketfunc.h"

struct Socket_Config {
    size_t connect_max = 0;
    size_t connect_nums = 0;
    size_t write_max = 0;
    int listen_port = 0;
    int listenfd = 0;
    Socket_Config& operator=(Socket_Config tmp) {
        this->connect_max = tmp.connect_max;
        this->connect_nums = tmp.connect_nums;
        this->write_max = tmp.write_max;
        this->listen_port = tmp.listen_port;
        this->listenfd = tmp.listenfd;
        return *this;
    }
};

class Socket_Control {
private:
    Log* socket_control_log;
    bool have_upper;        //have upper levels log pointer;
    Socket_Func socket_func;
    Socket_Config socket_config;

public:
    Socket_Control() {};
    void SetConfig(Socket_Config config) { socket_config = config; }
    void SetLog(Log *upper);
    int SocketListen();
    int SocketAccept();
    int SocketRead(int socketfd, std::string *str);      //read once;
    int SocketWrite(int socketfd, std::string *message); //write once;
    void SocketDisconnet();
    ~Socket_Control();
};

void Socket_Control::SetLog(Log* upper) {
    if (upper != nullptr) {
        socket_func_log = upper;
        have_upper = true;
    } else {
        socket_func_log = new Log(logfile, 200);
        have_upper = false;
    }
}

int Socket_Control::SocketListen() {
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(socket_config.listen_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    int listenfd = socket_config.listenfd;
    listenfd = socket_func.Socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        return -1;
    }
    if (socket_func.Bind(listenfd,
                        (struct sockaddr*)&server_address,
                         sizeof(server_address)) < 0) {
        return -1;
    }
    if (socket_func.Listen(listenfd, 0) < 0) {
        return -1;
    }
    signal(SIGPIPE, SIG_IGN);
    socket_config.listenfd = listenfd;
    return listenfd;
}

int Socket_Control::SocketAccept() {
    if (socket_config.connect_nums == socket_config.connect_max) {
        return -1;
    } else {
        return socket_func.Accept(socket_config.listenfd);
    }
}

int Socket_Control::SocketRead(int socketfd, std::string *readbuf) {
    return socket_func.Read(socketfd, readbuf);
}

int Socket_Control::SocketWrite(int socketfd, std::string *message) {
    int ret = 0;
    int writemax = socket_config.write_max;
    if (message->size() > writemax) {
        std::string tmp(*message, 0, writemax);
        *message = message->substr(writemax, (message->size() - writemax));
        ret = Servfunc::Write(socketfd, &tmp);
    } else {
        ret = Servfunc::Write(socketfd, message);
        message->clear();
    }
    return ret;
}

int Socket_Control::SocketDisconnet(int socketfd) {
    return socket_func.Close(socketfd);
}

Socket_Control::~Socket_Control() {
    if (!have_upper) {
        delete socket_control_log;
    }
}




#endif