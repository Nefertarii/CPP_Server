#include "Head/httpresponse.h"

using namespace Wasi::Http;

HttpRespone::HttpRespone(std::string http_request) :
    respone_head() {}

void HttpRespone::Swap(HttpRespone& other) {
    std::swap(respone_head, other.respone_head);
}

HttpHead HttpRespone::Get_respone_head() { return respone_head; }

HttpRespone::~HttpRespone() {}