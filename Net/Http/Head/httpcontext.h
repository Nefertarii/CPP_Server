#ifndef HTTP_CONTEXT_H_
#define HTTP_CONTEXT_H_

#include <memory>

namespace Wasi {
namespace Http {

class HttpRequest;
class HttpRespone;

class HttpContext {
private:
    enum class ParseState {
        PARSE,
        HEAD,
        BODY,
        ALLDONE,
    };
    // ParseState parse_state;
    std::shared_ptr<HttpRequest> request;
    std::shared_ptr<HttpRespone> respone;

public:
    HttpContext();
    void Set_request(std::string* http_request);
    // void Set_request_body(std::string* http_request);
    // ParseState Get_State();
    std::shared_ptr<HttpRespone> Get_respone();
    ~HttpContext();
};

}
} // namespace Wasi::Http

#endif // !HTTP_CONTEXT_H_