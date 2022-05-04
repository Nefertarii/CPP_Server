#include "Head/httprequest.h"

using namespace Wasi::Http;

HttpRequest::HttpRequest() :
    request(),
    method(Method::INVAILD),
    version(HttpVersion::UNKNOWN),
    path(),
    is_file(false),
    target(),
    code_num(HttpCode::CODE200),
    body() {}

HttpRequest::HttpRequest(std::string http_request) :
    request(),
    method(Method::INVAILD),
    version(HttpVersion::UNKNOWN),
    path(),
    is_file(false),
    target(),
    code_num(HttpCode::CODE200),
    body() {}

void HttpRequest::Parse() { Parse(request); }

void HttpRequest::Parse(std::string message) {
    // need add Accept-Charset paese
    if (message.empty()) { return; }
    std::string line;
    size_t msg_posi = 0;
    msg_posi        = message.find_first_of("\n"); // get first line
    if (msg_posi == 15) {
        method  = Method::GET;
        target  = "/index.html";
        path    = "";
        is_file = true;
        return;
    }
    if (msg_posi != std::string::npos) {
        line = message.substr(0, msg_posi);
    }
    if (line[0] == 'G') {
        method  = Method::GET;
        is_file = true;
        line.assign(line.begin() + 4, line.end());
    } else if (line[0] == 'P') {
        method  = Method::POST;
        is_file = false;
        line.assign(line.begin() + 5, line.end());
    } else {
        method = Method::INVAILD;
        return;
    }
    size_t tmp_posi = line.find_first_of(' ');
    if (tmp_posi != std::string::npos) { // http path
        line.assign(line.begin(), line.begin() + tmp_posi + 1);
        path     = line.substr(0, tmp_posi);
        tmp_posi = path.find_last_of('/');
        if (tmp_posi != std::string::npos) { // get path & target
            tmp_posi += 1;
            target = path.substr(tmp_posi, path.size());
            path.assign(path.begin(), path.begin() + tmp_posi);
        }
    }
    if (line.size() > 8) { // http version
        if (line[5] == '1') {
            if (line[7] == '0') {
                version = HttpVersion::HTTP10;
            } else if (line[7] == '1') {
                version = HttpVersion::HTTP11;
            }
        } else if (line[5] == '2') {
            if (line[7] == '0') {
                version = HttpVersion::HTTP20;
            }
        } else {
            version = HttpVersion::UNKNOWN;
        }
    }
    msg_posi = message.find_last_of("\r\n");
    if (msg_posi != std::string::npos) {
        body = message.substr(msg_posi + 1, message.size());
    }
}

void HttpRequest::Swap(HttpRequest& other) {
    request.swap(other.request);
    std::swap(method, other.method);
    std::swap(version, other.version);
    path.swap(other.path);
    std::swap(is_file, other.is_file);
    target.swap(other.target);
    std::swap(code_num, other.code_num);
    body.swap(other.body);
}

void HttpRequest::Init() {
    request.clear();
    method  = Method::INVAILD;
    version = HttpVersion::UNKNOWN;
    path.clear();
    is_file = false;
    target.clear();
    code_num = HttpCode::CODE200;
    body.clear();
}

HttpRequest::~HttpRequest() {}