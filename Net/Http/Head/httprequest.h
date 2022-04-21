#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "httpenum.h"

namespace Wasi {
namespace Http {

// using Headers = std::map<std::string, std::string>;

class HttpRequest {
private:
    std::string request;

    Method method;
    Version version;
    std::string path;
    std::string target;

    HttpCode code_num;
    std::string body;
    std::string modify_time;

public:
    HttpRequest();
    HttpRequest(std::string http_request);
    void Set_request(std::string http_request);
    void Set_code_num(HttpCode code);
    void Set_body(std::string body_);
    void Set_modify_time(std::string modify_time_);
    void Parse();
    void Parse(std::string message);
    HttpCode Get_code_num() const;
    Method Get_method() const;
    Version Get_version() const;
    std::string Get_path() const;
    std::string Get_body() const;
    std::string Get_target() const;
    std::string Get_modify_time() const;
    void Swap(HttpRequest& other);
    ~HttpRequest();
};

}
} // namespace Wasi::Http

#endif // !HTTP_REQUEST_H_