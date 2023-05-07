#include "Include/Web/Http/httpprocess.h"
#include "Include/Web/Http/httpcontext.h"
#include "Include/Web/Http/httprequest.h"
#include "Include/Web/Http/httprespone.h"
#include "Include/Net/tcpconnection.h"
#include "Include/Timer/clock.h"
#include "Include/Logger/logger.h"
#include <sys/stat.h>

using namespace wasi;
using namespace wasi::web;

std::string HttpProcess::html_dir_ = "/home/ubuntu/Server/Resource/HTML";
std::string HttpProcess::CRLF_     = "\r\n";

void HttpProcess::SetHtmlDir(std::string dir) {
    html_dir_ = dir;
}

std::string HttpProcess::GetHtmlDir() { return html_dir_; }

bool HttpProcess::ParseRequest(net::TcpConnectionPtr conn) {
    HttpContext context        = std::any_cast<HttpContext>(conn->GetContext());
    std::string origin_request = conn->GetOutputString();
    HttpRequestHead request_data;
    head::FileStat file_stat;
    if (origin_request.empty()) {
        LOG_INFO("request no data, from:" + conn->GetPeerAddress().GetIp() + " handle close");
        return false;
    }
    size_t pos = origin_request.find("\r\n\r\n");
    if (pos == std::string::npos) {
        LOG_INFO("read bad request from:" + conn->GetPeerAddress().GetIp());
        return false;
    }
    std::string request_line = origin_request.substr(0, origin_request.find("\r\n"));
    std::string request_head = origin_request.substr(request_line.length() + 2, pos - request_line.length() - 2);
    std::string request_body = origin_request.substr(pos + 4, origin_request.length() - pos - 4);
    { // request line
        std::string method, path, version;
        size_t end_pos;
        size_t start_pos            = request_line.find_first_of(' '); // request type
        method                      = request_line.substr(0, start_pos);
        request_data.request_method = TransHttpMethod(method);

        end_pos               = start_pos + 1;
        start_pos             = request_line.find_first_of(' ', start_pos + 1);
        file_stat.target_path = request_line.substr(end_pos, start_pos - end_pos);
        if (file_stat.target_path == "/") {
            file_stat.target_path = "/index.html";
        }
        end_pos              = start_pos + 1;
        version              = request_line.substr(end_pos);
        request_data.version = TransVersion(version);

        if (request_data.request_method == HttpMethod::kPost) {
            size_t pos2 = file_stat.target_path.find_last_of('/');
            std::string target_method =
                file_stat.target_path.substr(pos2, file_stat.target_path.length() - pos2);
            request_data.post_method = TransPostMethod(target_method);
        }
    }
    { // requset head
        std::map<std::string, std::string> headers;
        size_t start_pos = 0;
        while (start_pos != std::string::npos && start_pos < request_head.length()) {
            size_t end_pos          = request_head.find("\r\n", start_pos);
            std::string header_line = request_head.substr(start_pos, end_pos - start_pos);
            size_t header_delim_pos = header_line.find(":");
            if (header_delim_pos != std::string::npos) {
                std::string header_name  = header_line.substr(0, header_delim_pos);
                std::string header_value = header_line.substr(header_delim_pos + 1);
                headers[header_name]     = header_value;
            }
            if (end_pos == std::string::npos) {
                break;
            } else {
                start_pos = end_pos + 2;
            }
        }
        request_data.host       = headers["Host"];
        request_data.user_agent = headers["User-Agent"];
        request_data.accept     = headers["Accept"];

        if (request_data.host.empty()) {
            LOG_INFO("illegal peer request(no host) from:" + conn->GetPeerAddress().GetIp());
            return false;
        }
    }
    { // parse body
        request_data.request_body = request_body;
    }
    { // log
        LOG_INFO("peer_addr " + conn->GetPeerAddress().GetIp() + " request\r"
                 + "target:" + file_stat.target_path + "\r"
                 + "agent:" + request_data.user_agent);
    }
    HttpRequest request;
    request.SetFileStat(file_stat);
    request.SetRequestData(request_data);
    context.SetRequest(request);
    conn->SetContext(context);
    return true;
}

bool HttpProcess::ProcessRequestGet(net::TcpConnectionPtr conn) {
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRequest> request_ptr = context.GetRequest();
    std::shared_ptr<HttpRequest> request   = request_ptr.lock();
    HttpRequestHead request_data           = request->GetRequestData();
    head::FileStat conn_file_stat          = request->GetFileStat();
    bool return_flag;
    if (conn_file_stat.SetFileStat(html_dir_ + conn_file_stat.target_path)) {
        return_flag               = true;
        request_data.respone_code = HttpStateCode::k200;
    } else {
        return_flag               = false;
        request_data.respone_code = HttpStateCode::k403;
    }    request->SetRequestData(request_data);
    request->SetFileStat(conn_file_stat);
    conn->SetContext(context);
    return return_flag;
}

bool HttpProcess::ProcessRequestPost(net::TcpConnectionPtr conn) {
    return false;

    HttpContext context        = std::any_cast<HttpContext>(conn->GetContext());
    std::string origin_request = conn->GetOutputString();
    HttpRequestHead request_data;
    switch (request_data.post_method) {
    case PostMethod::kLogin:
        /* code */
        break;
    case PostMethod::kResetpwd:
        /* code */
        break;
    case PostMethod::kRegsiter:
        /* code */
        break;
    case PostMethod::kAccount:
        /* code */
        break;
    default:
        request_data.respone_code = HttpStateCode::k400;
        return false;
    }
    request_data.respone_code = HttpStateCode::k200;
    conn->SetContext(context);
    return true;
}

bool HttpProcess::SetBadRespone(net::TcpConnectionPtr conn) {
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRequest> request_ptr = context.GetRequest();
    std::shared_ptr<HttpRequest> request   = request_ptr.lock();
    request->GetRequestData().Clear();
    return true;
}

bool HttpProcess::SetBadPage(net::TcpConnectionPtr conn) {
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRequest> request_ptr = context.GetRequest();
    std::shared_ptr<HttpRequest> request   = request_ptr.lock();
    HttpRequestHead request_data;
    head::FileStat conn_file_stat;
    switch (request->GetRequestData().respone_code) {
    case HttpStateCode::k404:
        conn_file_stat.target_path = html_dir_ + "/Errorpage/Page404.html";
    default:
        break;
    }

    if (conn_file_stat.SetFileStat()) {
        request_data.respone_code = HttpStateCode::k200;
    } else {
        request_data.respone_code = HttpStateCode::k403;
    }
    request->SetFileStat(conn_file_stat);
    request->SetRequestData(request_data);
    conn->SetContext(context);
    return true;
}

bool HttpProcess::PrepareResponeHead(net::TcpConnectionPtr conn) {
    HttpContext context                  = std::any_cast<HttpContext>(conn->GetContext());
    std::shared_ptr<HttpRequest> request = context.GetRequest().lock();
    std::shared_ptr<HttpRespone> respone = context.GetRespone().lock();
    HttpRequestHead request_data         = request->GetRequestData();
    head::FileStat file_stat             = request->GetFileStat();
    HttpResponeHead respone_data;

    std::string version       = TransVersion(request_data.version);
    std::string code          = TransHttpStateCode(request_data.respone_code);
    respone_data.respone_line = version + " " + code + CRLF_;
    { // body
        ;
    }

    { // conntion
        if (request_data.keep_alive) {
            respone_data.connection         = "Connection: Keep-Alive\r\n";
            respone_data.connection_timeout = "Keep-Alive: timeout=" + std::to_string(request_data.keep_alive_timeout)
                                              + ", max=" + std::to_string(request_data.keep_alive_max) + CRLF_;
        }
    }

    { // content
        if (file_stat.file_length > 0) {
            respone_data.content_length = "Content-Length: " + std::to_string(file_stat.file_length) + CRLF_;
            respone_data.content_type   = "Content-Type: " + TransFileType(TransFileType(file_stat.target_type)) + CRLF_;
            respone_data.last_modified  = "Last-Modified: " + timer::Clock::ToStringSec(file_stat.last_modify, "%a, %d %b %G %T GMT\r\n");
        } else if (respone->GetResponeBody().length() > 0) {
            respone_data.content_length = "Content-Length: " + std::to_string(respone->GetResponeBody().size()) + CRLF_;
            respone_data.content_type   = "Content-Type: text/plain\r\n";
        } else {
            respone_data.content_length = "Content-Length: 0\r\n";
        }
    }

    respone_data.charset = "Charset: UTF-8\r\n";
    respone_data.date    = "Data: " + timer::Clock::ToStringSec(timer::Clock::NowtimeSec(), "%a, %d %b %G %T GMT\r\n");

    respone_data.HttpHeadMerge();
    respone->SetResponeFile(file_stat);
    respone->SetResponeHead(respone_data.respone_head);
    respone->SetResponeDate(respone_data);
    context.SetState(ResponeSendState::kHead);
    conn->SetContext(context);
    return true;
}

bool HttpProcess::ProcessRequest(net::TcpConnectionPtr conn) {
    bool flag = true;
    if (!ParseRequest(conn)) {
        SetBadRespone(conn);
        flag = false;
    }
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRequest> request_ptr = context.GetRequest();
    std::shared_ptr<HttpRequest> request   = request_ptr.lock();
    switch (request->GetRequestData().request_method) {
    case HttpMethod::kGet:
        if (!ProcessRequestGet(conn) && flag) {
            SetBadRespone(conn);
            flag = false;
        }
        break;
    case HttpMethod::kPost:
        if (!ProcessRequestPost(conn) && flag) {
            SetBadPage(conn);
            flag = false;
        }
        break;
    default:
        LOG_INFO("illegal peer request(no type) from:" + conn->GetPeerAddress().GetIp());
        return false;
    }
    PrepareResponeHead(conn);
    return true;
}

HttpProcess::~HttpProcess() {}