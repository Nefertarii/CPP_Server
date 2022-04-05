#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "httpenum.h"
#include <map>
#include <string>

namespace Wasi {
namespace Http {

// using Headers = std::map<std::string, std::string>;

class HttpRequest {
private:
    Method method;
    Version version;
    std::string path;
    std::string body;
    std::string target;
    HttpHead request_head;
    void Parse(std::string message);

public:
    HttpRequest(std::string http_request);
    Method Get_method() const;
    Version Get_version() const;
    std::string Get_path() const;
    std::string Get_body() const;
    std::string Get_target() const;
    void Swap(HttpRequest& other);
    ~HttpRequest();
};

}
} // namespace Wasi::Http

#endif // !HTTP_REQUEST_H_