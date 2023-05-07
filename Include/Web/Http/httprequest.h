#ifndef WEB_HTTP_HTTPREQUEST_H_
#define WEB_HTTP_HTTPREQUEST_H_

#include "Include/Web/Http/httphead.h"
#include "Include/Header/filestat.h"
#include <map>

namespace wasi {
namespace web {

class HttpRequest {
private:
    HttpRequestHead request_data_;
    head::FileStat file_stat_;
    std::string origin_request_;
    std::map<FileType, bool> accept_types_;

public:
    HttpRequest();
    HttpRequest(std::string request);
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    HttpRequestHead GetRequestData();
    std::string GetRequest();
    head::FileStat GetFileStat();
    std::map<FileType, bool> GetAcceptTypes();
    void SetRequestData(const HttpRequestHead& request_head);
    void SetHttpRequest(const std::string& request_str);
    void SetFileStat(const head::FileStat& file_stat);
    void SetAcceptTypes(const std::map<FileType, bool>& accept_types);
    // find the assigned variable in "other" and replace "this"
    void SetExistFields(const HttpRequest& other);
    void Clear();
    ~HttpRequest();
};

}} // namespace wasi::web

#endif