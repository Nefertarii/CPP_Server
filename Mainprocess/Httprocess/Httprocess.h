#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "../Servprocess.h"
#include "../Headfile.h"

void Http_signal_handle();

struct Clientinfo {
    std::string port;
    std::string ip;
    std::string respone_head;
    std::string respone_body;
    int clientfd;
    int socketfd;
    int filefd;
    SERVERR err_code;
    SERVSTATE state_code;
    REQUESTYPE requset_type;
};

//only encapsulates read and write operations
//need other function to control
class Httprocess
{
private:
    void Reset_client(struct Clientinfo client);

public:
    Httprocess(){}
    void Set_client(struct Clientinfo client);
    static int Send(int clientfd, std::string message);
    static int Sendfile(int clientfd, std::string filename);
    static int Sendfile(int clientfd, int filefd);
    int Read(int clientfd, std::string *read_buf);
    void Clear(struct Clientinfo client);
    void Disconnect(struct Clientinfo client);
    ~Httprocess(){};
};

class Httpconnect {
private:
    int concurrent_count;
    int connect_nums;

public:
    Httpconnect(){}
    void Connectlisten(int *listenfd);
    int Canconnect();
    const int Concurrent_count();
    const int Connect_nums();
    ~Httpconnect(){};
};



#endif