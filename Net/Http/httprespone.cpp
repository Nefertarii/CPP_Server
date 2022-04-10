#include "Head/httprespone.h"

using namespace Wasi::Http;

HttpRespone::HttpRespone(std::string http_request) :
    respone_head() {}

void HttpRespone::Swap(HttpRespone& other) {
    std::swap(respone_head, other.respone_head);
}

void HttpRespone::Set_code_num(std::string code_num) {
}

void HttpRespone::Set_server_name(std::string server_name) {
}

void HttpRespone::Set_connection_type(std::string connection_type) {
}

void HttpRespone::Set_content_type(std::string content_type) {
}

void HttpRespone::Set_content_length(std::string content_length) {
}

void HttpRespone::Set_last_modified(std::string last_modified) {
}

ResponeHead HttpRespone::Get_respone_head() { return respone_head; }

HttpRespone::~HttpRespone() {}