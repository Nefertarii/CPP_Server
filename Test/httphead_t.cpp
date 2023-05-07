#include "Include/Web/Http/httpprocess.h"
#include "Include/Net/tcpconnection.h"
#include "Include/Web/Http/httpcontext.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include <iostream>

using namespace wasi;
using namespace wasi::web;

void HttpRequestHead_t() {
    HttpRequestHead request_head_1;
    request_head_1.request_method = HttpMethod::kGet;
    request_head_1.post_method    = PostMethod::kLogin;
    request_head_1.version        = HttpVersion::kHttp10;
    request_head_1.respone_code   = HttpStateCode::k200;

    request_head_1.host         = "11111";
    request_head_1.accept       = "22222";
    request_head_1.user_agent   = "33333";
    request_head_1.request_body = "44444";
    HttpRequestHead request_head_2(request_head_1);
    std::cout << "\n";
    request_head_1.Clear();
    std::cout << "\n";
    request_head_1 = request_head_2;
    std::cout << "\n";
}

void HttpRequest_t() {
    HttpRequest http_request;
    HttpRequestHead request_data;
    request_data.request_method = HttpMethod::kGet;
    request_data.post_method    = PostMethod::kLogin;
    request_data.version        = HttpVersion::kHttp10;
    request_data.respone_code   = HttpStateCode::k200;
    http_request.SetRequestData(request_data);

    FileStat file_stat;
    file_stat.target_path = "path";
    file_stat.target_type = FileType::kHtml;
    file_stat.file_length = 1000;
    http_request.SetFileStat(file_stat);

    std::string origin_request;
    origin_request = "origin request";
    http_request.SetRequest(origin_request);

    std::map<FileType, bool> accept_types;
    accept_types[FileType::kHtml] = true;
    accept_types[FileType::kCss] = true;
    accept_types[FileType::kIcon] = false;
    http_request.SetAcceptTypes(accept_types);
    std::cout << "\n";

    request_data.Clear();
    file_stat.Clear();
    origin_request.clear();
    accept_types.clear();

    request_data = http_request.GetRequestData();
    file_stat    = http_request.GetFileStat();
    origin_request = http_request.GetRequest();
    accept_types   = http_request.GetAcceptTypes();
    std::cout << "\n";
}

void HttpRequest_t2() {
    HttpRequest http_request1, http_request2, http_request3;
    HttpRequestHead request_data1, request_data2, request_data3;
    request_data1.request_method = HttpMethod::kGet;
    request_data1.post_method    = PostMethod::kLogin;
    http_request1.SetRequestData(request_data1);

    request_data2.version        = HttpVersion::kHttp10;
    request_data2.respone_code   = HttpStateCode::k200;
    http_request2.SetRequestData(request_data2);

    //完全空，
    http_request3 = http_request1;
    std::cout << "\n";
    //有一些数据，
    http_request3.SetExistFields(http_request2);
    std::cout << "\n";
}

int main() {
    log::Logger::Init();
    //HttpRequestHead_t();
    HttpRequest_t();
    //HttpRequest_t2();
}