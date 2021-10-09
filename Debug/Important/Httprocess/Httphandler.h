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

int HTTP_Handler::MethodGetParse(Clientinfo* client, std::string readbuf) {
    std::string log;
    std::string filename, filetype;
    if (processctrl.GETParse(readbuf, &filename, &client->fileinfo) < 0) {
        this_log->Infolog("Request type:GET, Bad requeset.");
        client->http_state = Notfound;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Notfound.");
        return -1;
    } else {
        client->http_state = OK;
        processctrl.CreateResponeHead(client, processctrl.FileType(filename));
        log = "Request type:GET, Request file:" + filename + ".";
        this_log->Infolog(log);
        return 0;
    }
}

int HTTP_Handler::MethodPostParse(Clientinfo* client, std::string readbuf) {
    std::string log;
    std::string type, data;
    if (processctrl.POSTParse(readbuf, &type, &data) < 0) {
        this_log->Infolog("Request type:POST, Bad requeset.");
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        return -1;
    } else {
        client->postdata = data;
        client->requset_type = processctrl.POSTChoose(type);
        log = "Request type:POST. Request type:" + type + " Data:" + data + ".";
        this_log->Infolog(log);
        PostProcess(client);
        return 0;
    }
}

void HTTP_Handler::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p != nullptr) {
        this_log = log_p;
        have_upper = true;
    } else {
        this_log = new Log("Http_Handler_Log.txt", logbuf_size);
        have_upper = false;
    }
}

void HTTP_Handler::Init(std::map<std::string, std::string>* global_string_settings,
                        std::map<std::string, size_t>* global_value_settings) {
    logbuf_size = global_value_settings->find("MaxLogBuffer")->second;
    //process
    HttpHeadConfig respone_head_config;
    respone_head_config.Constent_Charset = global_string_settings->find("Constent_Charset")->second;
    respone_head_config.Content_Language = global_string_settings->find("Content-Language")->second;
    respone_head_config.Server_Name = global_string_settings->find("Server-Name")->second;
    processctrl.SetLog(this_log, logbuf_size);
    processctrl.SetHttpRespone(respone_head_config);
    processctrl.SetReadmax(global_value_settings->find("ReadMax")->second);
    processctrl.SetDocumentRoot(global_string_settings->find("DocumentRoot")->second);
    //respone
    responectrl.SetLog(this_log, logbuf_size);
    responectrl.SetReadmax(global_value_settings->find("ReadMax")->second);
    responectrl.SetWritemax(global_value_settings->find("WriteMax")->second);
    //account
    accountctrl.SetLog(this_log, logbuf_size);
    accountctrl.ReadAccountInfoFile(global_string_settings->find("AccountInfoFile")->second);
    //done
    this_log->Infolog("Http handler init complite.");
}

void HTTP_Handler::RequestParse(Clientinfo* client, std::string readbuf) {
    switch (processctrl.RequestType(&readbuf)) {
    case GET: {
        client->requset_type = GET;
        MethodGetParse(client, readbuf);
        break;
    } case POST: {
        client->requset_type = POST;
        MethodPostParse(client, readbuf);
        break;
    } default: {
        client->requset_type = TYPENONE;
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        this_log->Errorlog("Bad request type.");
        break;
    }
    }//switch end
}

void HTTP_Handler::PostProcess(Clientinfo* client) {
    std::vector<std::string> data;
    std::string tmp;
    int datasize = client->postdata.size();
    int index = 0;
    while (index <= datasize) {
        tmp = Substr(client->postdata, index, 100, '&');
        data.push_back(tmp);
        index += tmp.size() + 1;
    }
    switch (client->requset_type) {
    case POSTLogin: { //数据要求 邮箱&密码
        client->respone_body = "{\"Login\":[";
        if (accountctrl.Login(data[0], data[1])) {
            client->respone_body += JsonSpliced({ "state", "success" });
            client->respone_body += ",";
            client->respone_body += accountctrl.GetAccountInfo(data[0]);
        } else {
            client->respone_body += JsonSpliced({ "state", "false" });
        }
        client->respone_body += "]}";
        client->http_state = OK;
        processctrl.CreateResponeHead(client);
        break;
    }
    case POSTReset: { //数据要求 邮箱&旧密码&新密码
        client->respone_body = "{\"ResetPassword\":[";
        if (accountctrl.ChangePassword(data[0], data[1], data[2])) {
            client->respone_body += JsonSpliced({ "state","success" });
        } else {
            client->respone_body += JsonSpliced({ "state","false" });
        }
        client->respone_body += "]}";
        client->http_state = OK;
        processctrl.CreateResponeHead(client);
        this_log->Warninglog("Send request OK.");
        break;
    }
    case POSTRegister: { //数据要求 邮箱&密码&用户名
        client->respone_body = "{\"Regsiter\":[";
        if (accountctrl.Regsiter(data[0], data[1], data[2])) {
            client->respone_body += JsonSpliced({ "state","success" });
        } else {
            client->respone_body += JsonSpliced({ "state","false" });
        }
        client->respone_body += "]}";
        client->http_state = OK;
        processctrl.CreateResponeHead(client);
        this_log->Warninglog("Send request OK.");
        break;
    }
    case POSTVoteup: {
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        break;
    }
    case POSTVotedown: {
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        break;
    }
    case POSTComment: {
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        break;
    }
    case POSTContent: {
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request Forbidden.");
        break;
    }
    default: {
        client->http_state = Forbidden;
        processctrl.CreateBadHead(client);
        this_log->Warninglog("Send request 403.");
        break;
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