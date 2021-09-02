#include "Httprocess.h"




void Httprocess::Reset_client(struct Clientinfo *client) {
    client->port.clear();
    client->ip.clear();
    client->respone_head.clear();
    client->respone_body.clear();
    client->clientfd = 0;
    client->socketfd = 0;
    client->filefd = 0;
    client->err_code = ERRNONE;
    client->state_code = STATENONE;
    client->requset_type = TYPENONE;
}

void Httprocess::Set_client(struct Clientinfo *client) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(client->clientfd, (struct sockaddr *)&client_address, &address_length);
    client->port = std::to_string(ntohs(client_address.sin_port));
    client->ip = inet_ntoa(client_address.sin_addr);
}

int Httprocess::Send(int clientfd, std::string message) {
    int ret = SERV::Write(clientfd, message);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {  //error code set
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Sendfile(int clientfd, std::string filename) {
    struct Filestate file;
    SERV::Readfile(filename, &file);
    int ret = SERV::Writefile(clientfd, file.filefd, 10);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Sendfile(int clientfd, int filefd) {
    int ret = SERV::Writefile(clientfd, filefd, 10);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Read(int clientfd, std::string *read_buf) {
    int ret = SERV::Read(clientfd, read_buf);
    if (ret == 0) {
        return 0;
    }
    else if (ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

void Httprocess::Clear(struct Clientinfo client) {
    client.respone_head.clear();
    client.respone_body.clear();
    client.filefd = 0;
}

void Httprocess::Disconnect(struct Clientinfo *client) {
    shutdown(client->clientfd, SHUT_WR);
    Reset_client(client);
}




int Httpconnect::Connectlisten() {
    int listenfd = 0;
    concurrent_count = std::thread::hardware_concurrency();
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(LISTENPORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    listenfd = SERV::Socket(AF_INET, SOCK_STREAM, 0);
    SERV::Bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));
    SERV::Listen(listenfd, concurrent_count * SINGLECLIENTS);
    signal(SIGPIPE, SIG_IGN);
    return listenfd;
}

int Httpconnect::Canconnect() {
    if(connect_nums + 1 > concurrent_count * SINGLECLIENTS) {
        return -1;
    }
    else {
        connect_nums++;
        return 0;
    }
}

const int Httpconnect::Concurrent_count() { return concurrent_count; }

const int Httpconnect::Connect_nums() { return connect_nums; }

const int Httpconnect::Single_concurrent_client() { return SINGLECLIENTS; }




std::string Httprespone::Str_httpstate(int codenum) {
    switch (codenum) {
    case 200:
        return " OK\r\n";
    case 400:
        return " Bad Request\r\n";
    case 401:
        return " Unauthorized\r\n";
    case 403:
        return " Forbidden\r\n";
    case 404:
        return " Not Found\r\n";
    default:
        return " \r\n";
    }
}

std::string Httprespone::Filetype(std::string filename) {
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
    }
    else if (suffix == ".css") {
        return "text/css";
    }
    else if (suffix == ".js") {
        return "text/javascript";
    }
    else if (suffix == ".data") {
        return "application/json";
    }
    else if (suffix == ".png") {
        return "image/png";
    }
    else if (suffix == ".svg") {
        return "image/svg+xml";
    }
    else if (suffix == ".ico") {
        return "image/x-icon";
    }
    else {
        return "text/plain";
    }
}

void Httprespone::Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength) {
    *responehead += ("HTTP/1.1 " + std::to_string(state) + Str_httpstate(state)); 
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += ("Content-Type:" + filetype + "\r\n");
    *responehead += ("Content-Length:" + std::to_string(bodylength) + "\r\n");
    *responehead += (Timer::Nowtime_str() + "\r\n");
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

REQUESTYPE Httprespone::Requestparse(std::string *readbuf) {
    if(readbuf->find_first_of("GET") == 0) {
        return GET;
    }
    else if(readbuf->find_first_of("POST") == 0) {
        return POST;
    }
    else {
        return TYPENONE;
    }
}

int Httprespone::GETparse(std::string readbuf,std::string *filename) { //return GET request filename
    std::string filedir = FILEDIR;
    std::string request_file = Substr(readbuf, 5, READMAX, ' ');
    if (request_file == "0") {
        request_file = "index.html";

    }
    if (request_file == "-1") {
        Infolog("Get request fail");
        return -1;
    }
    *filename = filedir + request_file;
    std::string tmp = "Get request:" + request_file;
    Infolog(tmp);
    return 0;
}
 
int Httprespone::GETprocess(std::string filename,Filestate *file) {
    if(!SERV::Readfile(filename, file)) {
        return -1;
    }
    else {
        return 0;
    }
}

int Httprespone::POSTparse(std::string request, std::string *post_type, std::string *post_data) { //return POST request type and date
    std::string type = Substr(request, 5, READMAX, ' ');
    type = Substr_Revers(type, 30, '/');
    if(type == "-1" || type == "0") {
        return -1;
    }
    std::string data = Substr_Revers(request, READMAX, '\n');
    if(data == "-1" || data == "0") {
        return -1;
    }
    *post_type = type;
    *post_data = data;
    return 0;
}

POSTYPE Httprespone::POSTchoose(std::string post_type) {
    if (post_type == "login") {
        return POSTLogin;
    }
    else if (post_type == "reset") {
        return POSTReset;
    }
    else if (post_type == "register") {
        return POSTRegister;
    }
    else if (post_type == "vote_up") {
        return POSTVoteup;
    }
    else if (post_type == "vote_down") {
        return POSTVotedown;
    }
    else if (post_type == "comment") {
        return POSTComment;
    }
    else if (post_type == "content") {
        return POSTContent;
    }
    else if (post_type == "readcount") {
        return POSTReadcount;
    }
    else if (post_type == "verification") {
        return POSTVerifi;
    }
    else {
        return POSTERR;
    }
}

int Httprespone::POSTprocess(std::string post_type, std::string post_data) {
    POSTYPE post = POSTchoose(post_type);
    switch (post) {
    case POSTLogin: {
        std::string tmp = "Login " + post_data;
        Infolog(tmp);
        break;
    }

    case POSTReset: {
        std::string tmp = "Reset password " + post_data;
        Infolog(tmp);
        break;
    }
    
    case POSTRegister: {
        std::string tmp = "Register " + post_data;
        Infolog(tmp);
        break;
    }

    case POSTVoteup: {
        std::string tmp = "Vote up " + post_data;
        Infolog(tmp);
        break;
    }

    case POSTVotedown: {
        std::string tmp = "Vote down " + post_data;
        Infolog(tmp);
        break;
    }

    case POSTComment: {
        std::string tmp = "Comment " + post_data;
        Infolog(tmp);
        break;
    }

    case POSTContent: {
        std::string tmp = "Content " + post_data;
        Infolog(tmp);
        break;
    }
    
    case POSTReadcount: {
        Infolog("Readcount add");
        break;
    }

    case POSTVerifi: {
        std::string tmp = "Verifi " + post_data;
        Infolog(tmp);
        break;
    }

    default: {
        std::string tmp = "Login " + post_data;
        Infolog(tmp);
        break;
    }

    }
    return 0;
}
