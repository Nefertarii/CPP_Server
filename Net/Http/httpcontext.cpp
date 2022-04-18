#include "Head/httpcontext.h"
#include "Head/httprequest.h"
#include "Head/httprespone.h"

using namespace Wasi::Http;

HttpContext::HttpContext() :
    request(std::make_shared<HttpRequest>()),
    respone(std::make_shared<HttpRespone>()) {}

void HttpContext::Set_request(std::string* http_request) {
    request->Parse(http_request);
}

std::shared_ptr<HttpRespone> HttpContext::Get_respone() {
    return respone;
}

HttpContext::~HttpContext() {}