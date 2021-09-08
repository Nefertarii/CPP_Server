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
        ret = SERV::Write(socketfd, &tmp);
    } else {
        ret = SERV::Write(socketfd, message);
        message->clear();
    }
    return ret;
}

int Httprocess::Sendfile(int socketfd, Filestate *file) {
    int ret = 0;
    std::string log;
    if((file->filelength - file->offset) > WRITEMAX) {
        std::string log;
        if (file->offset == 0) {
            log = "Write file to: " + std::to_string(socketfd);
            Infolog(log);
        }
        ret = SERV::Writefile(socketfd, file->filefd, file->offset);
        file->offset += WRITEMAX;
    } else {
        ret = SERV::Writefile(socketfd, file->filefd, file->offset);
        file->filefd = 0;
        file->filelength = 0;
        file->offset = 0;
        if (file->offset == 0) {
            log = "Write file to: " + std::to_string(socketfd) + " success.";
            Infolog(log);
        }
    }
    return ret;
}

int Httprocess::Read(int socketfd, std::string *read_buf) {
    return SERV::Read(socketfd, read_buf);
}

void Httprocess::Clear(struct Clientinfo *client) {
    client->respone_head.clear();
    client->respone_body.clear();
    client->rewrite_count = 0;
    client->fileinfo.filefd = 0;
    client->fileinfo.filelength = 0;
    client->fileinfo.offset = 0;
}










