#include "Httprocess.h"

void Http_Process::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p == nullptr) {
        this_log = new Log("Http_Process_Log.txt", buffer_size);
        have_upper = false;
    } else {
        this_log = log_p;
        have_upper = true;
    }
}

std::string Http_Process::StrHttpState(int codenum) {
    switch (codenum) {
    case 200:
        return "OK\r\n";
    case 400:
        return "Bad Request\r\n";
    case 401:
        return "Unauthorized\r\n";
    case 403:
        return "Forbidden\r\n";
    case 404:
        return "Not Found\r\n";
    default:
        return "\r\n";
    }
}

std::string Http_Process::FileType(std::string filename) {
    int i = filename.length();
    while (i > 0) {
        if (filename[i] == '.' || i == 0) {
            break;
        }
        i--;
    }
    std::string suffix(filename, i, filename.length());
    if (suffix == ".html") {
        return "text/html";
    } else if (suffix == ".css") {
        return "text/css";
    } else if (suffix == ".js") {
        return "text/javascript";
    } else if (suffix == ".data") {
        return "application/json";
    } else if (suffix == ".png") {
        return "image/png";
    } else if (suffix == ".svg") {
        return "image/svg+xml";
    } else if (suffix == ".ico") {
        return "image/x-icon";
    } else {
        return "text/plain";
    }
}

void Http_Process::CreateResponeHead(Clientinfo* client, std::string filetype) {
    client->respone_head.clear();
    std::string code_num = std::to_string(client->http_state);
    std::string code_detail = StrCode::StrHttpCode(client->http_state);
    client->respone_head += "HTTP/1.1 " + code_num + " " + code_detail + "\r\n";
    client->respone_head += "Constent_Charset:" + respone_head_config.Constent_Charset + "\r\n";
    client->respone_head += "Content-Language:" + respone_head_config.Content_Language + "\r\n";
    if (filetype.size()) {
        client->respone_head += "Content-Type:" + filetype + "\r\n";
    }
    if (client->respone_body.size()) {
        client->respone_head += "Content-Length:" + std::to_string(client->respone_body.size()) + "\r\n";
    } else {
        client->respone_head += "Content-Length:" + std::to_string(client->fileinfo.filelength) + "\r\n";
    }

    client->respone_head += Timer::Nowtime_str() + "\r\n";
    client->respone_head += "Server version:" + respone_head_config.Server_Name + "\r\n";
    client->respone_head += "\r\n";
}

void Http_Process::CreateBadHead(Clientinfo* client) {
    client->respone_head.clear();
    std::string code_num = std::to_string(client->http_state);
    std::string code_detail = StrCode::StrHttpCode(client->http_state);
    client->respone_head += "HTTP/1.1 " + code_num + " " + code_detail + "\r\n";
    client->respone_head += "Constent_Charset:" + respone_head_config.Constent_Charset + "\r\n";
    client->respone_head += "Content-Language:" + respone_head_config.Content_Language + "\r\n";
    client->respone_head += "Content-Length: 0\r\n";
    client->respone_head += Timer::Nowtime_str() + "\r\n";
    client->respone_head += "Server version:" + respone_head_config.Server_Name + "\r\n";
    client->respone_head += "\r\n";
}

REQUESTYPE Http_Process::RequestType(std::string* readbuf) {
    if (readbuf->find_first_of("GET") == 0) {
        return GET;
    }
    if (readbuf->find_first_of("POST") == 0) {
        return POST;
    }
    return TYPENONE;
}

int Http_Process::GETParse(std::string readbuf, std::string* filename, Filestate* fileinfo) { //return GET request filename
    *filename = Substr(readbuf, 5, readmax, ' ');
    if (*filename == "-1") {
        return -1;
    }
    if (*filename == "0") {
        *filename = "index.html";
    }
    *filename = document_root + *filename;
    if (Gsocket::Readfile(*filename, fileinfo, this_log) < 0) {
        return -1;
    }
    return 0;
}

int Http_Process::POSTParse(std::string request, std::string* post_type, std::string* post_data) { //return POST request type and date
    //Get POST type
    size_t index = request.find("Content-Length:");
    //"Conte nt-Le ngth:&nbsp"
    std::string length = Substr(request, index + 16, readmax, '\n');
    size_t content_size = std::stoul(length);
    std::string type = Substr(request, 6, readmax, ' ');
    type = SubstrRevers(type, type.length(), '/');
    //Get readbuf data
    std::string data = request.substr((request.length() - content_size), content_size);
    if (data.size() == 0) { return -1; }
    data += "&";
    *post_type = type;
    *post_data = data;
    return 0;
}

REQUESTYPE Http_Process::POSTChoose(std::string post_type) {
    if (post_type == "login") {
        return POSTLogin;
    } else if (post_type == "reset") {
        return POSTReset;
    } else if (post_type == "register") {
        return POSTRegister;
    } else if (post_type == "vote_up") {
        return POSTVoteup;
    } else if (post_type == "vote_down") {
        return POSTVotedown;
    } else if (post_type == "comment") {
        return POSTComment;
    } else if (post_type == "content") {
        return POSTContent;
    } else if (post_type == "readcount") {
        return POSTReadcount;
    } else if (post_type == "verification") {
        return POSTVerifi;
    } else {
        return TYPENONE;
    }
}

Http_Process::~Http_Process() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}