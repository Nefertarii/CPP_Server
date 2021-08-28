#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "../Servprocess.h"
#include "../Headfile.h"

void HTTPhandle();

//only encapsulates read and write operations
//need other function control
class Httprocess {
private:
    SERVERR err_code_;
    SERVSTATE state_code_;
    REQUESTYPE requset_type_;
    const char *Str_error(int codenum);
    const char *Str_state(int codenum);
    void Reset();

public:
    Httprocess(){};
    Httprocess(int socketfd);
    void Set_clientfd(int socketfd);
    int Clientfd();
    int Send(std::string message);
    static int Send(std::string message, std::string clientip, int clientfd);
    //static int Send(std::string message, Httprocess client);

    int Sendfile(std::string filename);
    static int Sendfile(std::string filename, std::string clientip, int clientfd);
    //static int Sendfile(std::string filename, Httprocess client);

    int Sendfile(int filefd);
    static int Sendfile(int filefd, std::string clientip, int clientfd);
    //static int Sendfile(int filefd, Httprocess client);

    int Read(std::string *read_buf);
    void Disconnect();
    ~Httprocess(){};
};

class Httpconnect {
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