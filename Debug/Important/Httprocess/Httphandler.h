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

int HTTP_Handler::MethodGetParse(Clientinfo* client, std::string readbuf) {
    std::string log;
    std::string filename, filetype;
    if (processctrl.GETParse(readbuf, &filename, &client->fileinfo) < 0) {
        log = "Request:GET bad requeset.";
        this_log->Infolog(log);
        return -1;
    } else {
        filetype = processctrl.FileType(filename);
        processctrl.CreateResponeHead(&client->respone_head, filetype, 200, client->fileinfo.filelength);
        log = "Request:GET. Request file:" + filename + ".";
        this_log->Infolog(log);
        return 0;
    }
}

int HTTP_Handler::MethodPostParse(Clientinfo* client, std::string readbuf) {
    std::string log;
    std::string type, data;
    if (processctrl.POSTParse(readbuf, &type, &data) < 0) {
        log = "Request:POST bad requeset.";
        this_log->Infolog(log);
        return -1;
    } else {
        log = "Request:POST. Request type:" + type + ".Data:" + data + ".";
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

    if (socket_settings.listenfd < 0) {
        this_log->Infolog("Http handler init fail.");
        return;
    }
    this_log->Infolog("Http handler init complite.");
}

void HTTP_Handler::RequestParse(Clientinfo* client, std::string readbuf) {
    switch (processctrl.RequestType(&readbuf)) {
    case GET: {
        if (!MethodGetParse(client, readbuf)) {
            responectrl.BadRequest404(&client->respone_head);
        }
    } case POST: {
        if (!MethodPostParse(client, readbuf)) {
            responectrl.BadRequest404(&client->respone_head);
        }
        responectrl.BadRequest404(&client->respone_head); // not use post;
    } default: {
        responectrl.BadRequest404(&client->respone_head);
        this_log->Errorlog("Bad request type.");
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
    if (!have_upper) {
        delete this_log;
        this_log = nullptr;
    }
}


#endif