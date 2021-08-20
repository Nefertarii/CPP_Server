#ifndef SERVPROCESS_H_
#define SERVPROCESS_H_

#include "Headfile.h"
#include "Protocol.h"

class Connectaccept {
private:
    int trytime;
    int sockfd;

public:
    Connectaccept();
    void Client_socket();
    void Client_listen();
    void Client_bind();
    void Client_accept();
    void Client_close();
    ~Connectaccept();
};

class Connectprocess {
private:
    std::string readbuf;
    std::string respone_head;
    std::string respone_body;
    SERVERR err_code;
    SERVSTATE state_code;
    REQUESTYPE requset_type;
    const char *Str_error(int codenum);
    const char *Str_state(int codenum);

public:
    Connectprocess();
    void Send();
    void Sendfile();
    void Get();
    void Disconnect();
    ~Connectprocess();
};
#endif