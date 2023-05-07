#ifndef WEB_HTTP_HTTPRESPONE_H_
#define WEB_HTTP_HTTPRESPONE_H_

#include "Include/Web/Http/httphead.h"
#include "Include/Header/filestat.h"

namespace wasi {
namespace web {

class HttpRespone {
private:
    // HTTP/<version> <status code> <status message>
    std::string respone_head_;
    // <headers>
    std::string respone_line_;
    // <response body>
    std::string respone_body_;
    head::FileStat respone_file_;
    HttpResponeHead reaspone_data_;

public:
    HttpRespone();

    HttpRespone(const HttpRespone& other);
    HttpRespone& operator=(const HttpRespone& other);
    std::string GetResponeHead();
    std::string GetResponeLine();
    std::string GetResponeBody();
    head::FileStat GetResponeFile();
    HttpResponeHead GetResponeDate();
    void SetResponeHead(const std::string& respone_head);
    void SetResponeLine(const std::string& respone_line);
    void SetResponeBody(const std::string& respone_body);
    void SetResponeFile(const head::FileStat& respone_file);
    void SetResponeDate(const HttpResponeHead& date);
    void Clear();
    ~HttpRespone();
};

}
} // namespace wasi::web

#endif