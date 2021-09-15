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
    std::string document_root;
    Http_Process processctrl;
    Http_Respone responectrl;
    void ClientClear();
    int MethodGetParse(Clientinfo* client, std::string readbuf);
    int MethodPostParse(Clientinfo* client, std::string readbuf);
    int SendResponehead(Clientinfo* client);
    int SendResponebody(Clientinfo* client);
    int SendResponefile(Clientinfo* client);
public:
    HTTP_Handler() {}
    void Init(Log* log_p, size_t logbuf_size, std::string document_root_, Socket_Config socket_settings);
    void RequestParse(Clientinfo* client, std::string readbuf);
    int SendRespone(Clientinfo* client);
    ~HTTP_Handler();
};

#endif