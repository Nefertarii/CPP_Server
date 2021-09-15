#ifndef GSOCKETCTRL_H_
#define GSOCKETCTRL_H_

#include "Gsocketfunc.h"

struct Socket_Config {
    size_t connect_max = 0;
    size_t connect_nums = 0;
    size_t write_max = 0;
    size_t read_max = 0;
    int listen_port = 0;
    int listenfd = 0;
    int reuseaddr = 0;
    int reuseport = 0;
    Socket_Config& operator=(Socket_Config tmp) {
        this->connect_max = tmp.connect_max;
        this->connect_nums = tmp.connect_nums;
        this->write_max = tmp.write_max;
        this->read_max = tmp.read_max;
        this->listen_port = tmp.listen_port;
        this->listenfd = tmp.listenfd;
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
    void SetConfig(Socket_Config* config) { socket_config = config; }
    void SetLog(Log* upper, size_t buffer_size);
    int SocketListen();
    int SocketAccept();
    int SocketRead(int socketfd, std::string* str);      //read once;
    int SocketWrite(int socketfd, std::string* message); //write once;
    int SocketDisconnet(int socketfd);
    ~Socket_Control();
};





#endif