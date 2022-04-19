#include "Head/httprequest.h"

using namespace Wasi::Http;

HttpRequest::HttpRequest() :
    method(Method::INVAILD),
    version(Version::UNKNOWN),
    path(),
    target(),
    body() {}

HttpRequest::HttpRequest(std::string http_request) :
    request(nullptr),
    method(Method::INVAILD),
    version(Version::UNKNOWN),
    path(),
    target(),
    body() {}

void HttpRequest::Set_request(std::string http_request) { request = http_request; }

void HttpRequest::Paese() { Parse(request); }

void HttpRequest::Parse(std::string message) {
    if (message.empty()) { return; }
    std::string line;
    size_t msg_posi = 0;
    msg_posi        = message.find_first_of("\n"); // get first line
    if (msg_posi != std::string::npos) {
        line = message.substr(0, msg_posi);
    }
    if (line[0] == 'G') {
        method = Method::GET;
        line.assign(line.begin() + 4, line.end());
    } else if (line[0] == 'P') {
        method = Method::POST;
        line.assign(line.begin() + 5, line.end());
    } else {
        method = Method::INVAILD;
        return;
    }
    size_t tmp_posi = line.find_first_of(' ');
    if (tmp_posi != std::string::npos) { // http path
        line.assign(line.begin() + tmp_posi + 1, line.end());
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
                version = Version::HTTP10;
            } else if (line[7] == '1') {
                version = Version::HTTP11;
            }
        } else if (line[5] == '2') {
            if (line[7] == '0') {
                version = Version::HTTP20;
            }
        } else {
            version = Version::UNKNOWN;
        }
    }
    msg_posi = message.find_last_of("\r\n");
    if (msg_posi != std::string::npos) {
        body = message.substr(msg_posi + 1, message.size());
    }
}

Method HttpRequest::Get_method() const { return method; }

Version HttpRequest::Get_version() const { return version; }

std::string HttpRequest::Get_path() const { return path; }

std::string HttpRequest::Get_body() const { return body; }

std::string HttpRequest::Get_target() const { return target; }

void HttpRequest::Swap(HttpRequest& other) {
    std::swap(method, other.method);
    std::swap(version, other.version);
    path.swap(other.path);
    target.swap(other.target);
    body.swap(other.body);
}

HttpRequest::~HttpRequest() {}