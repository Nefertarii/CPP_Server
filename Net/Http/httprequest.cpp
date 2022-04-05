#include "Head/httprequest.h"

//    Method method;
//    Version version;
//    std::string path;
//    std::string body;
//    std::string target;
//
using namespace Wasi::Http;

void HttpRequest::Parse(std::string message) {
}

HttpRequest::HttpRequest(std::string message) :
    method(),
    version(),
    path(),
    body(),
    target() {
    Parse(message);
}
Method HttpRequest::Get_method() const;
Version HttpRequest::Get_version() const;
std::string HttpRequest::Get_path() const;
std::string HttpRequest::Get_body() const;
std::string HttpRequest::Get_target() const;
void HttpRequest::Swap(HttpRequest& other);
HttpRequest::~HttpRequest();