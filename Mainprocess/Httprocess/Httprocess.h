#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "Servfunc.h"

void Http_signal_handle();

//only encapsulates read and write operations
//need other function to control
class Httprocess {
private:
    ;

public:
    Httprocess(){}
    void Set_client(struct Clientinfo *client);
    static int Send(int socketfd, std::string *message);

    //static int Sendfile(int clientfd, std::string filename);
    //static int Sendfile(int socketfd, int filefd, off_t offset);

    static int Sendfile(int socketfd, Filestate *file);
    int Read(int socketfd, std::string *read_buf);
    void Clear(struct Clientinfo *client);
    ~Httprocess(){};
};


#endif