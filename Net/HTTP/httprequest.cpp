#include "Head/httprequest.h"

using namespace Wasi::Http;

HttpRequest::HttpRequest() :
    request(),
    method(Method::INVAILD),
    version(HttpVersion::UNKNOWN),
    path(),
    target(),
    code_num(HttpCode::CODE200),
    body(),
    is_file(false),
    parse_state(false) {}

HttpRequest::HttpRequest(std::string http_request) :
    request(),
    method(Method::INVAILD),
    version(HttpVersion::UNKNOWN),
    path(),
    target(),
    code_num(HttpCode::CODE200),
    body(),
    is_file(false),
    parse_state(false) {}

void HttpRequest::Parse() { Parse(request); }

void HttpRequest::Parse(std::string message) {
    // need add Accept-Charset paese
    if (message.empty()) { return; }
    std::string line;
    size_t msg_posi = 0;
    msg_posi        = message.find_first_of("\n");
    if (msg_posi == std::string::npos) {
        return;
    } else if (msg_posi == 15) {
        method      = Method::GET;
        target      = "/index.html";
        path        = "";
        is_file     = true;
        parse_state = true;
        return;
    }
    line = message.substr(0, msg_posi); // get first line: "GET /index.html HTTP/1.1\r\n"

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

    msg_posi = message.find_last_of("\r\n");
    if (msg_posi != std::string::npos) {
        body = message.substr(msg_posi + 1, message.size());
    }

    size_t tmp_posi = line.find_first_of(' ');
    path            = line.substr(0, tmp_posi); // get request position
    line.assign(line.begin(), line.begin() + tmp_posi + 1);

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

    if (tmp_posi != std::string::npos) {
        if (method == Method::GET) { // http path
            tmp_posi = path.find_last_of('/');
            if (tmp_posi != std::string::npos) { // get path & target
                tmp_posi += 1;
                target = path.substr(tmp_posi, path.size());
                path.assign(path.begin(), path.begin() + tmp_posi);
            }
        } else if (method == Method::POST) {
            if (path == "/login") {
                post_method = PostMethod::LOGIN;
            } else if (path == "/reset") {
                post_method = PostMethod::RESET;
            } else if (path == "/register") {
                post_method = PostMethod::REGISTER;
            } else if (path == "/vote") {
                post_method = PostMethod::VOTE;
            } else if (path == "/comment") {
                post_method = PostMethod::COMMENT;
            } else if (path == "/content") {
                post_method = PostMethod::CONTENT;
            } else {
                post_method = PostMethod::UNKNOWN;
                return;
            }
            // else if (path == "/readcount") {
            //     post_method = PostMethod::;
            // } else if (path == "/verification") {
            //     post_method = PostMethod::;
            // }
        }
    }

    parse_state = true;
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
    method      = Method::INVAILD;
    post_method = PostMethod::UNKNOWN;
    version     = HttpVersion::UNKNOWN;
    path.clear();
    is_file = false;
    target.clear();
    code_num = HttpCode::CODE200;
    body.clear();
}

HttpRequest::~HttpRequest() {}