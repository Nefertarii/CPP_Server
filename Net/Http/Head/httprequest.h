#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "httpenum.h"

namespace Wasi {
namespace Http {

// using Headers = std::map<std::string, std::string>;

class HttpRequest {
public:
    std::string request;

    Method method;
    HttpVersion version;
    std::string path;
    bool is_file;
    std::string target;

    HttpCode code_num;
    std::string body;

    HttpRequest();
    HttpRequest(std::string http_request);
    void Parse();
    void Parse(std::string message);
    void Swap(HttpRequest& other);
    void Init();
    ~HttpRequest();
};

}
} // namespace Wasi::Http

#endif // !HTTP_REQUEST_H_