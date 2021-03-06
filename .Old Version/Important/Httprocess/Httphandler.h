#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_

#include "Httprocess.h"
#include "Httprespone.h"
#include "Accountconfirm.h"
#include "../Gstring.h"
#include <map>

class HTTP_Handler {
private:
    size_t logbuf_size; //log
    Log* this_log;
    bool have_upper;
    std::string document_root;
    Http_Process processctrl;
    Http_Respone responectrl;
    Account_Parse accountctrl;
    void ClientClear();
    int MethodGetParse(Clientinfo* client, std::string readbuf);
    int MethodPostParse(Clientinfo* client, std::string readbuf);
    int SendResponehead(Clientinfo* client);
    int SendResponebody(Clientinfo* client);
    int SendResponefile(Clientinfo* client);
public:
    HTTP_Handler() {}
    void SetLog(Log* log_p, size_t logbuf_size);
    void Init(std::map<std::string, std::string>* global_string_settings_,
              std::map<std::string, size_t>* global_value_settings_);
    void RequestParse(Clientinfo* client, std::string readbuf);
    void PostProcess(Clientinfo* client);
    int SendRespone(Clientinfo* client);
    ~HTTP_Handler();
};

#endif