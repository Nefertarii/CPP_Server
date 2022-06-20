#include "httpcontext.h"
#include "httprequest.h"
#include "httprespone.h"

using namespace Wasi::Http;

HttpContext::HttpContext() :
    request(std::make_shared<HttpRequest>()),
    respone(std::make_shared<HttpRespone>()) {}

// void HttpContext::Set_State(ParseState state) { parse_state = state; }

std::shared_ptr<HttpRequest> HttpContext::Get_request() { return request; }

std::shared_ptr<HttpRespone> HttpContext::Get_respone() { return respone; }

HttpContext::~HttpContext() {}