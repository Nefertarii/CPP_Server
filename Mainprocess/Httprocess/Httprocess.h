#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "../Servprocess.h"
#include "../Headfile.h"

void HTTPhandle();

//only encapsulates read and write operations
//need other function control
class Httprocess: public Connectprocess {
private:
    int clientfd;
    std::string Client_ip;
    std::string Client_port;
    SERVERR err_code;
    SERVSTATE state_code;
    REQUESTYPE requset_type;
    const char *Str_error(int codenum);
    const char *Str_state(int codenum);
    void Reset();

public:
    Httprocess(){};
    Httprocess(int socketfd);
    void Set_clientfd(int socketfd);
    int Clientfd();
    static int Send(std::string message);
    static int Sendfile(std::string filename);
    static int Sendfile(int filefd);
    int Read(std::string *read_buf);
    void Disconnect();
    ~Httprocess(){};
};

class Httpconnect : public Connectaccept {
private:
    int socketfd;
    int listenfd;
    unsigned concurrent_count;
    unsigned connect_count;
public:
    Httpconnect();
    void Connectlisten();
    int Client_accept();
    const int Socketfd();
    const int Listenfd();
    const int Concurrentcount();
    const int Clientcount();
    ~Httpconnect(){};
};



#endif