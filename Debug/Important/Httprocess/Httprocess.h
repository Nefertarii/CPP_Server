#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "../Gservfunc.h"
#include "Httperrno.h"

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










#include "Httprocess.h"

extern const size_t WRITEMAX;
 
void Httprocess::Set_client(struct Clientinfo *client) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(client->clientfd, (struct sockaddr *)&client_address, &address_length);
    client->port = std::to_string(ntohs(client_address.sin_port));
    client->ip = inet_ntoa(client_address.sin_addr);
}

int Httprocess::Send(int socketfd, std::string *message) {
    int ret = 0;
    if (message->size() > WRITEMAX) {
        std::string tmp(*message, 0, WRITEMAX);
        *message = message->substr(WRITEMAX, (message->size() - WRITEMAX));
        ret = Servfunc::Write(socketfd, &tmp);
    } else {
        ret = Servfunc::Write(socketfd, message);
        message->clear();
    }
    return ret;
}

int Httprocess::Sendfile(int socketfd, Filestate *file) {
    int ret = 0;
    if((file->filelength - file->offset) > WRITEMAX) {
        ret = Servfunc::Writefile(socketfd, file->filefd, file->offset);
        file->offset += WRITEMAX;
    } else {
        ret = Servfunc::Writefile(socketfd, file->filefd, file->offset);
        file->filefd = 0;
        file->filelength = 0;
        file->offset = 0;
    }
    return ret;
}

int Httprocess::Read(int socketfd, std::string *read_buf) {
    return Servfunc::Read(socketfd, read_buf);
}

void Httprocess::Clear(struct Clientinfo *client) {
    client->respone_head.clear();
    client->respone_body.clear();
    client->rewrite_count = 0;
    client->fileinfo.filefd = 0;
    client->fileinfo.filelength = 0;
    client->fileinfo.offset = 0;
}


#endif









