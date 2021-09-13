#ifndef HTTPCONNECT_H_
#define HTTPCONNECT_H_

#include "../Gservfunc.h"
#include "../Gthread.h"
#include "Httperrno.h"


class Httpconnect {
private:
    size_t concurrent_count;
    size_t connect_nums;
    void Reset_client(struct Clientinfo *client);
    Log httpconnect_log();

public:
    Httpconnect(){}
    int Connectlisten();
    int Canconnect();
    const int Concurrent_count();
    const int Connect_nums();
    const int Single_concurrent_client();
    void Disconnect(struct Clientinfo *client);
    ~Httpconnect(){};
};






extern const size_t LISTENPORT; //listen port
extern const size_t SINGLECLIENTS;

void Httpconnect::Reset_client(struct Clientinfo *client) {
    client->port.clear();
    client->ip.clear();
    client->respone_head.clear();
    client->respone_body.clear();
    client->clientfd = 0;
    client->rewrite_count = 0;
    client->fileinfo.filefd = 0;
    client->fileinfo.filelength = 0;
    client->fileinfo.offset = 0;
    client->err_code = ERRNONE;
    client->state_code = STATENONE;
    client->requset_type = TYPENONE;
}

int Httpconnect::Connectlisten() {
    int listenfd = 0;
    concurrent_count = std::thread::hardware_concurrency();
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(LISTENPORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    listenfd = Servfunc::Socket(AF_INET, SOCK_STREAM, 0);
    Servfunc::Bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));
    Servfunc::Listen(listenfd, concurrent_count * SINGLECLIENTS);
    signal(SIGPIPE, SIG_IGN);
    return listenfd;
}

int Httpconnect::Canconnect() {
    if(connect_nums + 1 > concurrent_count * SINGLECLIENTS) {
        return -1;
    } else {
        connect_nums++;
        return 0;
    }
}

const int Httpconnect::Concurrent_count() { return concurrent_count; }

const int Httpconnect::Connect_nums() { return connect_nums; }

const int Httpconnect::Single_concurrent_client() { return SINGLECLIENTS; }

void Httpconnect::Disconnect(struct Clientinfo *client) {
    shutdown(client->clientfd, SHUT_RDWR);
    Servfunc::Close(client->clientfd);
    Reset_client(client);
    connect_nums -= 1;
}



#endif