#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

//#include "../Servprocess.h"
#include "Servfunc.h"

void Http_signal_handle();

struct Clientinfo {
    std::string port;
    std::string ip;
    std::string respone_head;
    std::string respone_body;
    //client's socketfd
    int clientfd;
    size_t writecount;
    Filestate fileinfo;
    SERVERR err_code;
    SERVSTATE state_code;
    REQUESTYPE requset_type;
};

//only encapsulates read and write operations
//need other function to control
class Httprocess {
private:
    void Reset_client(struct Clientinfo *client);
public:
    Httprocess(){}
    void Set_client(struct Clientinfo *client);
    static int Send(int socketfd, std::string *message);

    //static int Sendfile(int clientfd, std::string filename);
    //static int Sendfile(int socketfd, int filefd, off_t offset);

    static int Sendfile(int socketfd, Filestate *file);
    int Read(int socketfd, std::string *read_buf);
    void Clear(struct Clientinfo *client);
    void Disconnect(struct Clientinfo *client);
    ~Httprocess(){};
};

class Httpconnect {
private:
    size_t concurrent_count;
    size_t connect_nums;
    const size_t SINGLECLIENTS = 8;    
    const size_t LISTENPORT = 8000;   
public:
    Httpconnect(){}
    int Connectlisten();
    int Canconnect();
    const int Concurrent_count();
    const int Connect_nums();
    const int Single_concurrent_client();
    ~Httpconnect(){};
};

class Httprespone {
private:
    const size_t READMAX = 1024 * 4;                      //once read max length;
    const char *FILEDIR = "/home/cs18/vscode/Blog/";           //Default location for read file

public:
    std::string Str_httpstate(int codenum);
    std::string Filetype(std::string filename);
    void Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength);
    REQUESTYPE Requestparse(std::string *readbuf);
    int GETparse(std::string readbuf, std::string *filename);
    int GETprocess(std::string filename, Filestate *file);
    int POSTparse(std::string request, std::string *post_type, std::string *post_data);
    POSTYPE POSTchoose(std::string post_type);
    int POSTprocess(std::string post_type, std::string post_data);
};

#endif