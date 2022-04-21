#include "Head/httprespone.h"

using namespace Wasi::Http;

HttpRespone::HttpRespone() :
    respone_head() {}

void HttpRespone::Swap(HttpRespone& other) {
    std::swap(respone_head, other.respone_head);
}

void HttpRespone::Set_code_num(std::string code_num) {
    respone_head.code_num = code_num;
}

void HttpRespone::Set_server_name(std::string server_name) {
    respone_head.server_name = server_name;
}

void HttpRespone::Set_connection_type(std::string connection_type) {
    respone_head.connection_type = connection_type;
}

void HttpRespone::Set_content_type(std::string content_type) {
    respone_head.content_type = content_type;
}

void HttpRespone::Set_content_length(std::string content_length) {
    respone_head.content_length = content_length;
}

void HttpRespone::Set_last_modified(std::string last_modified) {
    respone_head.last_modified = last_modified;
}

ResponeHead HttpRespone::Get_respone_head() { return respone_head; }

std::string HttpRespone::Get_respone_body() { return respone_body; }

HttpRespone::~HttpRespone() {}