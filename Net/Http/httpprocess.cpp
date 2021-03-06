#include "httpprocess.h"
#include "httpaccount.h"
#include "httpcontext.h"
#include "httprequest.h"
#include "httprespone.h"
#include "httpserver.h"
#include <Base/Timer/clock.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpconnection.h>
#include <regex>
#include <sys/stat.h>

using namespace Wasi;
using namespace Http;

HttpAccount HttpProcess::account(account_dir);
std::regex HttpProcess::email_pattern("([A-Za-z0-9_\\-\\.])+\\@([A-Za-z0-9_\\-\\.])+\\.([A-Za-z]{2,4})");
std::regex HttpProcess::passwd_pattern("[a-zA-Z0-9_]{6,16}");
std::regex HttpProcess::name_pattern("[a-zA-Z0-9_]{4,16}");

int HttpProcess::Parse_request(const Server::TcpConnectionPtr& conn) {
    // Preaccess phase: IP control(black list)
    // ...
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    conn_request->Parse(conn->Get_input_buffer()->Content());
    if (conn_request->parse_state == false) {
        return -1;
    }
    return 0;
}

int HttpProcess::Get_process(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    // Base::FileStat* conn_file_stat            = conn->Get_file_stat();
    if (conn_request->is_file) {
        struct stat sys_file_stat;
        conn_respone->respone_file = html_dir + conn_request->path + conn_request->target;
        LOG_DEBUG("File:" + conn_respone->respone_file);
        if (stat(conn_respone->respone_file.c_str(), &sys_file_stat) < 0) {
            LOG_ERROR("Can't find file:" + conn_respone->respone_file);
            return -1;
        }
        size_t dot = conn_request->target.find_first_of('.');
        std::string file_type(conn_request->target, dot);
        if (file_type == ".html") {
            conn_respone->respone_head.content_type = FileType::HTML;
        } else if (file_type == ".css") {
            conn_respone->respone_head.content_type = FileType::CSS;
        } else if (file_type == ".js") {
            conn_respone->respone_head.content_type = FileType::JAVASCRIPT;
        } else if (file_type == ".json") {
            conn_respone->respone_head.content_type = FileType::JSON;
        } else if (file_type == ".png") {
            conn_respone->respone_head.content_type = FileType::PNG;
        } else if (file_type == ".svg") {
            conn_respone->respone_head.content_type = FileType::SVG;
        } else if (file_type == ".ico") {
            conn_respone->respone_head.content_type = FileType::ICON;
        } else {
            conn_respone->respone_head.content_type = FileType::PLAIN;
        }
        timespec file_modify_time                 = sys_file_stat.st_mtim;
        conn_respone->respone_head.last_modified  = file_modify_time.tv_sec;
        conn_respone->respone_head.date           = Time::Clock::Nowtime_sec();
        conn_respone->respone_head.code_num       = HttpCode::CODE200;
        conn_respone->respone_head.content_length = sys_file_stat.st_size;
        return 0;
    } else {
        return -1;
    }
}

int HttpProcess::Post_process(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    switch (conn_request->post_method) {
    case PostMethod::LOGIN: { // email & password
        LOG_DEBUG("post login");
        LOG_DEBUG(conn_request->body);
        size_t body_length = conn_request->body.size();
        if (body_length <= 4 || body_length > 54) {
            LOG_DEBUG("user login data length illegal");
            return -1;
        }
        size_t and1 = conn_request->body.find_first_of('&');
        if (and1 == std::string::npos) {
            LOG_DEBUG("user login data illegal");
            return -1;
        }
        std::string email(conn_request->body, 0, and1);
        std::string password(conn_request->body, and1 + 1, body_length);
        std::string user_id = account.Login(email, password);
        if (user_id.empty()) {
            LOG_DEBUG("user login fail, not this user");
            return -1;
        }
        AccountInfo user_info      = account.Get_account_by_id(user_id);
        conn_respone->respone_body = "{\"state\":\"success\",";
        conn_respone->respone_body += "\"AccountImage\":\"";
        conn_respone->respone_body += user_info.user_image;
        conn_respone->respone_body += "\",";
        conn_respone->respone_body += "\"AccountAlias\":\"";
        conn_respone->respone_body += user_info.user_alias;
        conn_respone->respone_body += "\"}";
        conn_respone->respone_head.content_type = FileType::JSON;
        break;
    }
    case PostMethod::RESET: { // email & oldpassword & password
        LOG_DEBUG("post reset");
        break;
    }
    case PostMethod::REGISTER: { // email & password & username
        LOG_DEBUG("post register");
        LOG_DEBUG(conn_request->body);
        size_t body_length = conn_request->body.size();
        if (body_length <= 10 || body_length > 73) {
            LOG_DEBUG("user register data length illegal");
            return -1;
        }
        size_t and1 = conn_request->body.find_first_of('&');
        std::string email(conn_request->body, 0, and1);
        and1 += 1;
        size_t and2 = conn_request->body.find_first_of('&', and1);
        std::string password(conn_request->body, and1, and2 - and1);
        and2 += 1;
        std::string alias(conn_request->body, and2, body_length);
        if (std::regex_match(email, email_pattern) == false) {
            LOG_DEBUG("user register email illegal");
            return -1;
        }
        if (std::regex_match(password, passwd_pattern) == false) {
            LOG_DEBUG("user register password illegal");
            return -1;
        }
        if (std::regex_match(alias, name_pattern) == false) {
            LOG_DEBUG("user register alias illegal");
            return -1;
        }
        if (account.Regsiter(email, password, alias) == false) {
            LOG_INFO("user register fail, same user already exists");
            return -1;
        }
        conn_respone->respone_body              = "{\"state\":\"success\"}";
        conn_respone->respone_head.content_type = FileType::JSON;
        break;
    }
    case PostMethod::ACCOUNTFIND: {
        LOG_DEBUG("post find account");
        if (account.Find_account_email(conn_request->body) == false) {
            conn_respone->respone_body              = "{\"state\":\"success\"}";
            conn_respone->respone_head.content_type = FileType::JSON;
            break;
        }
        return -1;
    }
    default: {
        LOG_DEBUG("post unknown");
        return -1;
    }
    }
    // fill HttpRequest
    conn_respone->respone_head.code_num       = HttpCode::CODE200;
    conn_respone->respone_head.content_length = conn_respone->respone_body.size();
    conn_respone->respone_head.date           = Time::Clock::Nowtime_sec();
    return 0;
}

int HttpProcess::Process_request(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    if (conn_request->version == HttpVersion::HTTP11) {
        conn_respone->respone_head.http_version       = HttpVersion::HTTP11;
        conn_respone->respone_head.connection         = ConnectionType::KEEPALIVE;
        conn_respone->respone_head.keep_alive_timeout = 5;
        conn_respone->respone_head.keep_alive_max     = 1000;
    } else if (conn_request->version == HttpVersion::HTTP10) {
        conn_respone->respone_head.http_version = HttpVersion::HTTP10;
    }
    if (conn_request->method == Method::GET) {
        if (Get_process(conn) < 0) {
            return -1;
        }
        return 0;
    } else if (conn_request->method == Method::POST) {
        if (Post_process(conn) < 0) {
            return -1;
        }
        return 0;
    }
    return -1;
}

int HttpProcess::Prepare_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    // Base::FileStat* conn_file_stat            = conn->Get_file_stat();
    std::string tmp_string;
    conn_respone->prepare_respone_head.clear();
    // version code
    tmp_string.clear();
    switch (conn_respone->respone_head.http_version) {
    case HttpVersion::HTTP10:
        tmp_string = "HTTP/1.0 ";
        break;
    case HttpVersion::HTTP11:
        tmp_string = "HTTP/1.1 ";
        break;
    case HttpVersion::HTTP20:
        tmp_string = "HTTP/2.0 ";
        break;
    default:
        tmp_string = "HTTP/1.0 ";
        break;
    }

    switch (conn_respone->respone_head.code_num) {
    case HttpCode::CODE200:
        tmp_string += "200 OK\r\n";
        break;
    case HttpCode::CODE301:
        tmp_string += "301 Moved Permanently\r\n";
        break;
    case HttpCode::CODE304:
        tmp_string += "304 Not Modified\r\n";
        break;
    case HttpCode::CODE400:
        tmp_string += "400 Bad Request\r\n";
        break;
    case HttpCode::CODE403:
        tmp_string += "403 Forbidden\r\n";
        break;
    case HttpCode::CODE404:
        tmp_string += "404 Not Found\r\n";
        break;
    case HttpCode::CODE411:
        tmp_string += "411 Length Required\r\n";
        break;
    case HttpCode::CODE500:
        tmp_string += "500 Internal Server Error\r\n";
        break;
    case HttpCode::CODE501:
        tmp_string += "501 Method Not Implemented\r\n";
        break;
    default:
        tmp_string += "404 Not Found\r\n";
        break;
    }
    conn_respone->prepare_respone_head += tmp_string;

    switch (conn_respone->respone_head.connection) {
    case ConnectionType::KEEPALIVE:
        tmp_string = "Connection: Keep-Alive\r\n";
        tmp_string += "Keep-Alive: timeout=";
        tmp_string += std::to_string(conn_respone->respone_head.keep_alive_timeout);
        tmp_string += ", max=";
        tmp_string += std::to_string(conn_respone->respone_head.keep_alive_max);
        tmp_string += "\r\n";
        break;
    default: tmp_string = ""; break;
    }
    conn_respone->prepare_respone_head += tmp_string;

    if (conn_respone->respone_head.content_length > 0) {
        tmp_string = "Content-Length: ";
        tmp_string += std::to_string(conn_respone->respone_head.content_length);
        tmp_string += "\r\n";
        conn_respone->prepare_respone_head += tmp_string;

        tmp_string = "Content-Type: ";
        switch (conn_respone->respone_head.content_type) {
        case FileType::HTML:
            tmp_string += "text/html; ";
            break;
        case FileType::CSS:
            tmp_string += "text/css; ";
            break;
        case FileType::JAVASCRIPT:
            tmp_string += "text/javascript; ";
            break;
        case FileType::JSON:
            tmp_string += "application/json; ";
            break;
        case FileType::PNG:
            tmp_string += "image/png; ";
            break;
        case FileType::SVG:
            tmp_string += "image/svg+xml; ";
            break;
        case FileType::ICON:
            tmp_string += "image/x-icon; ";
            break;
        case FileType::PLAIN:
            tmp_string += "text/plain; ";
            break;
        default:
            tmp_string += "text/plain; ";
            break;
        }

        switch (conn_respone->respone_head.charset) {
        case HttpCharset::UTF8:
            tmp_string += "charset=UTF-8\r\n";
            break;
        default:
            tmp_string += "charset=UTF-8\r\n";
            break;
        }
        conn_respone->prepare_respone_head += tmp_string;
    }

    if (conn_respone->respone_head.last_modified > 0) {
        tmp_string = "Last-Modified: ";
        tmp_string += Time::Clock::To_string_sec(conn_respone->respone_head.last_modified, "%a, %d %b %G %T GMT\r\n");
        conn_respone->prepare_respone_head += tmp_string;
    }

    tmp_string = "date: ";
    tmp_string += Time::Clock::To_string_sec(conn_respone->respone_head.date, "%a, %d %b %G %T GMT\r\n");
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "Server: " + conn_respone->respone_head.server + "\r\n";
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "\r\n";
    conn_respone->prepare_respone_head += tmp_string;

    return 0;
}

int HttpProcess::Send_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    conn->Send(conn_respone->prepare_respone_head);
    return 0;
}

int HttpProcess::Set_404_page(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    conn_request->Init();
    conn_respone->Init();

    conn_request->path                      = "/Errorpage";
    conn_request->target                    = "/Page404.html";
    conn_request->is_file                   = true;
    conn_respone->respone_head.http_version = HttpVersion::HTTP10;

    Get_process(conn);

    return 0;
}

int HttpProcess::Send_bad_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    if (conn_request->method == Method::GET) {
        Set_404_page(conn);
        Prepare_respone(conn);
        LOG_INFO("Get process error, Send 404 Page");
    } else { // Post
        conn_request->Init();
        conn_respone->Init();

        conn_respone->prepare_respone_head = "HTTP/1.0 403 Forbidden\r\n";
        conn_respone->respone_head.date    = Time::Clock::Nowtime_sec();
        conn_respone->prepare_respone_head += "Content-Length: 0\r\n";
        conn_respone->prepare_respone_head += "date: ";
        conn_respone->prepare_respone_head += Time::Clock::To_string_sec(conn_respone->respone_head.date, "%a, %d %b %G %T GMT\r\n");
        conn_respone->prepare_respone_head += "\r\n";

        LOG_INFO("Post processed error, Send bad respone");
    }
    Send_respone(conn);
    return 0;
}

void HttpProcess::Request_process(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    std::string msg                           = conn->Get_peer_address().To_string_ip_port();
    // parse_request
    if (Parse_request(conn) < 0) {
        msg += " Request phase: parse request fail";
        LOG_ERROR(msg);
        Send_bad_respone(conn);
        return;
    }
    // process_request
    if (Process_request(conn) < 0) {
        msg += " Request phase fail: processed request";
        LOG_ERROR(msg);
        Send_bad_respone(conn);
        return;
    }
    // prepare_respone
    if (Prepare_respone(conn) < 0) {
        msg += " Request phase fail: prepare respone";
        LOG_ERROR(msg);
        Send_bad_respone(conn);
        return;
    }
    // send_respone
    if (Send_respone(conn) < 0) {
        msg += " Request phase fail: send respone";
        LOG_ERROR(msg);
        return;
    }
    msg += " Request processed successfully.";
    LOG_INFO(msg);
}
