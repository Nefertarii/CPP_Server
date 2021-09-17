#ifndef GSOCKETCTRL_H_
#define GSOCKETCTRL_H_

#include "Gsocketfunc.h"

struct Socket_Config {
    bool is_server = true;
    size_t connect_max = 0;
    size_t connect_nums = 0;
    size_t write_max = 0;
    size_t read_max = 0;
    int port = 0;
    int socketfd = 0;
    int reuseaddr = 0;
    int reuseport = 0;
    Socket_Config& operator=(Socket_Config tmp) {
        this->is_server = tmp.is_server;
        this->connect_max = tmp.connect_max;
        this->connect_nums = tmp.connect_nums;
        this->write_max = tmp.write_max;
        this->read_max = tmp.read_max;
        this->port = tmp.port;
        this->socketfd = tmp.socketfd;
        this->reuseaddr = tmp.reuseaddr;
        this->reuseport = tmp.reuseport;
        return *this;
    }
};

class Socket_Control {
private:
    Log* this_log;
    bool have_upper;        //have upper levels log pointer;
    Socket_Config* socket_config;

public:
    Socket_Control() {};
    void SetConfig(Socket_Config* config);
    void SetLog(Log* upper, size_t buffer_size);
    int SocketConnect(std::string ip, int port);
    int SocketListen();
    int SocketAccept();
    int SocketRead(int socketfd, std::string* str);      //read once;
    int SocketWrite(int socketfd, std::string* message); //write once;
    int SocketDisconnet(int socketfd);
    ~Socket_Control();
};

void Socket_Control::SetConfig(Socket_Config* config) {
    if (config != nullptr) {
        socket_config = config;
    } else {
        std::cout << "\nConfig error!\n";
        return;
    }
}

void Socket_Control::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        this_log = upper;
        have_upper = true;
    } else {
        this_log = new Log("Socket_Control_Log.txt", buffer_size);
        have_upper = false;
    }
}

int Socket_Control::SocketConnect(std::string ip, int port) {
    if (socket_config->is_server) {
        this_log->Warninglog("Socket control set for Server.");
        return -1;
    } else {
        int socketfd = 0;
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        socketfd = Gsocket::Socket(AF_INET, SOCK_STREAM, 0, this_log);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
        Gsocket::Connect(socketfd, (sockaddr*)&servaddr, sizeof(servaddr), this_log);
        socket_config->socketfd = socketfd;
        return 0;
    }
}

int Socket_Control::SocketListen() {
    if (socket_config->is_server) {
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(socket_config->port);
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        int listenfd = socket_config->socketfd;
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
        socket_config->socketfd = listenfd;
        return listenfd;
    } else {
        this_log->Warninglog("Socket control set for Client.");
        return -1;
    }
}

int Socket_Control::SocketAccept() {
    if (socket_config->is_server) {
        if (socket_config->connect_nums == socket_config->connect_max) {
            return -1;
        } else {
            return Gsocket::Accept(socket_config->socketfd, this_log);
        }
    } else {
        this_log->Warninglog("Socket control set for Client.");
        return -1;
    }
}

int Socket_Control::SocketRead(int socketfd, std::string* readbuf) {
    return Gsocket::Read(socketfd, readbuf, socket_config->read_max, this_log);
}

int Socket_Control::SocketWrite(int socketfd, std::string* message) {
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

int Socket_Control::SocketDisconnet(int socketfd) {
    return Gsocket::Close(socketfd, this_log);
}

Socket_Control::~Socket_Control() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}

#endif