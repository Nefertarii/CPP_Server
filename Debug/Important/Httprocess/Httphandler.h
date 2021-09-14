#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_

#include "Httprocess.h"
#include "Httprespone.h"
#include "../Gsocketctrl.h"

class HTTP_Handler {
private:
    size_t buffer_size; //log
    Log* this_log;
    bool have_upper;
    Http_Process processctrl;
    Http_Respone responectrl;
    void ClientClear();
public:
    HTTP_Handler();
    int Listenfd() { return socket_settings.listenfd; }
    void Init(Log* log_p, size_t logbuf_size, Socket_Config socket_settings);
    int RequestParse(Clientinfo* client, std::string readbuf);
    void MethodGetParse(Clientinfo* client, std::string* readbuf);
    void MethodPostParse(Clientinfo* client, std::string* readbuf);
    void SendResponehead(Clientinfo* client);
    void SendResponebody(Clientinfo* client);
    void SendResponefile(Clientinfo* client);
    ~HTTP_Handler();
};

void HTTP_Handler::Init(Log* log_p, size_t logbuf_size, Socket_Config socket_settings) {
    buffer_size = logbuf_size;
    if (log_p != nullptr) {
        this_log = log_p;
        have_upper = true;
    } else {
        this_log = new Log("Http_Handler_Log.txt", buffer_size);
        have_upper = false;
    }
    //process
    processctrl.SetLog(this_log, buffer_size);
    processctrl.SetReadmax(socket_settings.read_max);
    //respone
    responectrl.SetLog(this_log, buffer_size);
    responectrl.SetReadmax(socket_settings.read_max);
    responectrl.SetWritemax(socket_settings.write_max);

    socket_settings.listenfd = connectctrl.SocketListen();
    if (socket_settings.listenfd < 0) {
        this_log->Infolog("Http handler init fail.");
        return;
    }
    this_log->Infolog("Http handler init complite.");
}

HTTP_Handler::HTTP_Handler() {
    ;
}

int HTTP_Handler::RequestParse(Clientinfo* client, std::string readbuf) {
    processctrl.RequestType(readbuf);
}

void HTTP_Handler::MethodPostParse(Clientinfo* client, std::string* readbuf) {
    ;
}
void HTTP_Handler::SendResponehead(Clientinfo* client) {
    ;
}
void HTTP_Handler::SendResponebody(Clientinfo* client) {
    ;
}
void HTTP_Handler::SendResponefile(Clientinfo* client) {
    ;
}
HTTP_Handler::~HTTP_Handler() {
    ;
}


#endif