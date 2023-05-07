#include "Include/Web/Http/httphead.h"
#include "Include/Logger/logger.h"
#include <sys/stat.h>

using namespace wasi;
using namespace wasi::web;

std::string web::TransHttpStateCode(HttpStateCode state_code) {
    std::string state;
    switch (state_code) {
    case HttpStateCode::k200:
        state = "200 OK";
        break;
    case HttpStateCode::k301:
        state = "301 Moved Permanently";
        break;
    case HttpStateCode::k304:
        state = "304 Not Modified";
        break;
    case HttpStateCode::k400:
        state = "400 Bad Request";
        break;
    case HttpStateCode::k403:
        state = "403 Forbidden";
        break;
    case HttpStateCode::k404:
        state = "404 Not Found";
        break;
    case HttpStateCode::k411:
        state = "411 Length Required";
        break;
    case HttpStateCode::k500:
        state = "500 Internal Server Error";
        break;
    case HttpStateCode::k501:
        state = "501 Method Not Implemented";
        break;
    default:
        state = "400 Not Found";
        break;
    }
    return state;
}

std::string web::TransFileType(FileType file_type) {
    std::string type;
    switch (file_type) {
    case FileType::kHtml:
        type = "text/html";
        break;
    case FileType::kCss:
        type = "text/css";
        break;
    case FileType::kJavascript:
        type = "text/javascript";
        break;
    case FileType::kJson:
        type = "application/json";
        break;
    case FileType::kPng:
        type = "image/png";
        break;
    case FileType::kSvg:
        type = "image/svg+xml";
        break;
    case FileType::kIcon:
        type = "image/x-icon";
        break;
    case FileType::kPlain:
        type = "text/plain";
        break;
    default:
        type = "text/plain";
        break;
    }
    return type;
}

FileType web::TransFileType(std::string str_file_type) {
    FileType type = FileType::kPlain;
    if (str_file_type == ".html") {
        type = FileType::kHtml;
    } else if (str_file_type == ".css") {
        type = FileType::kCss;
    } else if (str_file_type == ".js") {
        type = FileType::kJavascript;
    } else if (str_file_type == ".json") {
        type = FileType::kJson;
    } else if (str_file_type == ".png") {
        type = FileType::kPng;
    } else if (str_file_type == ".svg") {
        type = FileType::kSvg;
    } else if (str_file_type == ".ico") {
        type = FileType::kIcon;
    } else {
        type = FileType::kPlain;
    }
    return type;
}

PostMethod web::TransPostMethod(std::string str_post_type) {
    PostMethod type = PostMethod::kInvaild;
    if (str_post_type == "login") {
        type = PostMethod::kLogin;
    } else if (str_post_type == "reset") {
        type = PostMethod::kResetpwd;
    } else if (str_post_type == "register") {
        type = PostMethod::kRegsiter;
    } else if (str_post_type == "regsiter_email") {
        type = PostMethod::kAccount;
    } else {
        type = PostMethod::kInvaild;
    }
    return type;
}

HttpMethod web::TransHttpMethod(std::string str_method) {
    HttpMethod method = HttpMethod::kInvaild;
    if (str_method == "GET") {
        method = HttpMethod::kGet;
    } else if (str_method == "POST") {
        method = HttpMethod::kPost;
    } else if (str_method == "HEAD") {
        method = HttpMethod::kHead;
    } else if (str_method == "PUT") {
        method = HttpMethod::kPut;
    } else if (str_method == "DELETE") {
        method = HttpMethod::kDelete;
    } else if (str_method == "TRACE") {
        method = HttpMethod::kTrace;
    } else {
        method = HttpMethod::kInvaild;
    }
    return method;
}

std::string web::TransVersion(HttpVersion version) {
    std::string str_version;
    if (version == HttpVersion::kHttp10) {
        str_version = "HTTP/1.0";
    } else if (version == HttpVersion::kHttp11) {
        str_version = "HTTP/1.1";
    } else if (version == HttpVersion::kHttp20) {
        str_version = "HTTP/2.0";
    } else {
        str_version = "HTTP/1.0";
    }
    return str_version;
}

HttpVersion web::TransVersion(std::string str_version) {
    HttpVersion version = HttpVersion::kUnknown;
    if (str_version == "HTTP/1.0") {
        version = HttpVersion::kHttp10;
    } else if (str_version == "HTTP/1.1") {
        version = HttpVersion::kHttp11;
    } else if (str_version == "HTTP/2.0") {
        version = HttpVersion::kHttp20;
    } else {
        version = HttpVersion::kUnknown;
    }
    return version;
}

HttpRequestHead::HttpRequestHead() :
    keep_alive_timeout(5.0),
    keep_alive_max(100.0),
    request_method(HttpMethod::kInvaild),
    post_method(PostMethod::kInvaild),
    version(HttpVersion::kUnknown),
    respone_code(HttpStateCode::k403),
    host("\0"),
    accept("\0"),
    user_agent("\0"),
    request_body("\0"),
    keep_alive(false) {}

HttpRequestHead::HttpRequestHead(const HttpRequestHead& other) {
    this->request_method = other.request_method;
    this->post_method    = other.post_method;
    this->version        = other.version;
    this->respone_code   = other.respone_code;
    this->host           = other.host;
    this->accept         = other.accept;
    this->user_agent     = other.user_agent;
    this->request_body   = other.request_body;
    this->keep_alive     = other.keep_alive;
}

void HttpRequestHead::SetExistFields(const HttpRequestHead& other) {
    if (other.request_method != HttpMethod::kInvaild) {
        this->request_method = other.request_method;
    }
    if (other.post_method != PostMethod::kInvaild) {
        this->post_method = other.post_method;
    }
    if (other.version != HttpVersion::kUnknown) {
        this->version = other.version;
    }
    if (other.respone_code != HttpStateCode::k403) {
        this->version = other.version;
    }
    if (!other.host.empty()) {
        this->version = other.version;
    }
    if (!other.accept.empty()) {
        this->accept = other.accept;
    }
    if (!other.user_agent.empty()) {
        this->user_agent = other.user_agent;
    }
    if (!other.request_body.empty()) {
        this->request_body = other.request_body;
    }
    if (other.keep_alive != false) {
        this->keep_alive = other.keep_alive;
    }
}

void HttpRequestHead::Clear() {
    request_method = HttpMethod::kInvaild;
    post_method    = PostMethod::kInvaild;
    version        = HttpVersion::kUnknown;
    respone_code   = HttpStateCode::k403;
    host.clear();
    accept.clear();
    user_agent.clear();
    request_body.clear();
    keep_alive = false;
}

HttpRequestHead& HttpRequestHead::operator=(const HttpRequestHead& other) {
    this->request_method = other.request_method;
    this->post_method    = other.post_method;
    this->version        = other.version;
    this->respone_code   = other.respone_code;
    this->host           = other.host;
    this->accept         = other.accept;
    this->user_agent     = other.user_agent;
    this->request_body   = other.request_body;
    this->keep_alive     = other.keep_alive;
    return *this;
}

HttpResponeHead::HttpResponeHead() :
    respone_line("\0"),
    connection("\0"),
    connection_timeout("\0"),
    content_length("\0"),
    content_type("\0"),
    last_modified("\0"),
    charset("\0"),
    date("\0"),
    server_name("Server: Wasi's Server\r\n"),
    respone_head("\0"),
    respone_body("\0") {}

HttpResponeHead::HttpResponeHead(const HttpResponeHead& other) {
    this->respone_line       = other.respone_line;
    this->connection         = other.connection;
    this->connection_timeout = other.connection_timeout;
    this->content_length     = other.content_length;
    this->content_type       = other.content_type;
    this->last_modified      = other.last_modified;
    this->charset            = other.charset;
    this->date               = other.date;
    this->server_name        = other.server_name;
    this->respone_head       = other.respone_head;
    this->respone_body       = other.respone_body;
}

void HttpResponeHead::HttpHeadMerge() {
    respone_head += respone_line;
    respone_head += connection;
    respone_head += content_length;
    if (!content_type.empty()) {
        respone_head += content_type;
    }
    if (!last_modified.empty()) {
        respone_head += last_modified;
    }
    respone_head += charset;
    respone_head += date;
    respone_head += server_name;
    respone_head += "\r\n";
}

void HttpResponeHead::SetExistFields(const HttpResponeHead& other) {
    if (!other.respone_line.empty()) {
        this->respone_line = other.respone_line;
    }
    if (!other.connection.empty()) {
        this->connection = other.connection;
    }
    if (!other.connection_timeout.empty()) {
        this->connection_timeout = other.connection_timeout;
    }
    if (!other.content_length.empty()) {
        this->content_length = other.content_length;
    }
    if (!other.content_type.empty()) {
        this->content_type = other.content_type;
    }
    if (!other.last_modified.empty()) {
        this->last_modified = other.last_modified;
    }
    if (!other.charset.empty()) {
        this->charset = other.charset;
    }
    if (!other.date.empty()) {
        this->date = other.date;
    }
    if (!other.server_name.empty()) {
        this->server_name = other.server_name;
    }
    if (!other.respone_head.empty()) {
        this->respone_head = other.respone_head;
    }
    if (!other.respone_body.empty()) {
        this->respone_body = other.respone_body;
    }
}

void HttpResponeHead::Clear() {
    respone_line.clear();
    connection.clear();
    connection_timeout.clear();
    content_length.clear();
    content_type.clear();
    last_modified.clear();
    charset.clear();
    date.clear();
    respone_head.clear();
    respone_body.clear();
}

HttpResponeHead& HttpResponeHead::operator=(const HttpResponeHead& other) {
    this->respone_line       = other.respone_line;
    this->connection         = other.connection;
    this->connection_timeout = other.connection_timeout;
    this->content_length     = other.content_length;
    this->content_type       = other.content_type;
    this->last_modified      = other.last_modified;
    this->charset            = other.charset;
    this->date               = other.date;
    this->server_name        = other.server_name;
    this->respone_head       = other.respone_head;
    this->respone_body       = other.respone_body;
    return *this;
}
