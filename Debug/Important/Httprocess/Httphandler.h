#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_

#include "Httprocess.h"
#include "Httprespone.h"
#include "../Gsocketfunc.h"

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
    int MethodPostParse(Clientinfo* client, std::string readbuf, std::string* type, std::string* data);
    int SendResponehead(Clientinfo* client);
    int SendResponebody(Clientinfo* client);
    int SendResponefile(Clientinfo* client);
public:
    HTTP_Handler() {}
    void Init(Log* log_p, size_t logbuf_size, std::string document_root_, Socket_Config socket_settings);
    std::string RequestParse(Clientinfo* client, std::string readbuf);
    int SendRespone(Clientinfo* client);
    ~HTTP_Handler();
};

int HTTP_Handler::MethodGetParse(Clientinfo* client, std::string readbuf) {
    std::string log;
    std::string filename, filetype;
    if (processctrl.GETParse(readbuf, &filename, &client->fileinfo) < 0) {
        log = "Request type:GET, Bad requeset.";
        this_log->Infolog(log);
        return -1;
    } else {
        filetype = processctrl.FileType(filename);
        processctrl.CreateResponeHead(&client->respone_head, filetype, 200, client->fileinfo.filelength);
        log = "Request type:GET, Request file:" + filename + ".";
        this_log->Infolog(log);
        return 0;
    }
}

int HTTP_Handler::MethodPostParse(Clientinfo* client, std::string readbuf,
                                  std::string* type, std::string* data) {
    std::string log;
    if (processctrl.POSTParse(readbuf, type, data) < 0) {
        log = "Request type:POST, Bad requeset.";
        this_log->Infolog(log);
        return -1;
    } else {
        log = "Request type:POST. Request type:" + *type + ".Data:" + *data + ".";
        this_log->Infolog(log);
        return 0;
    }
}

void HTTP_Handler::Init(Log* log_p, size_t logbuf_size, std::string document_root_, Socket_Config socket_settings) {
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
    processctrl.SetDocumentRoot(document_root_);
    //respone
    responectrl.SetLog(this_log, buffer_size);
    responectrl.SetReadmax(socket_settings.read_max);
    responectrl.SetWritemax(socket_settings.write_max);

    this_log->Infolog("Http handler init complite.");
}

std::string HTTP_Handler::RequestParse(Clientinfo* client, std::string readbuf) {
    switch (processctrl.RequestType(&readbuf)) {
    case GET: {
        if (MethodGetParse(client, readbuf) < 0) {
            this_log->Warninglog("send bad request 403.");
            responectrl.BadRequest403(&client->respone_head);
        }
        return "ok";
    } case POST: {
        std::string type, data;
        if (MethodPostParse(client, readbuf, &type, &data) < 0) {
            this_log->Warninglog("send bad request 403.");
            responectrl.BadRequest403(&client->respone_head);
            this_log->Errorlog("Bad request type.");
            return "error";
        }
        return "post/" + type + "/" + data;
    } default: {
        this_log->Warninglog("send bad request 403.");
        responectrl.BadRequest403(&client->respone_head);
        this_log->Errorlog("Bad request type.");
        return "error";
    }
    }//switch end
}

int HTTP_Handler::SendRespone(Clientinfo* client) {
    if (!client->respone_head.empty()) {
        if (responectrl.Send(client->clientfd, &client->respone_head) < 0) {
            return -1;
        }
        client->respone_head.clear();
        return 1;
    }
    if (!client->respone_body.empty()) {
        if (responectrl.Send(client->clientfd, &client->respone_body) < 0) {
            return -1;
        }
        if (client->respone_body.empty()) {
            return 0;
        } else {
            return 1;
        }
    }
    if (client->fileinfo.filefd != 0) {
        if (responectrl.SendFile(client->clientfd, &client->fileinfo) < 0) {
            return -1;
        }
        if (client->fileinfo.filefd == 0) {
            return 0;
        } else {
            return 1;
        }
    }
    return 0;
}

HTTP_Handler::~HTTP_Handler() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}


#endif