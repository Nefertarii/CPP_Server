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
    // void Set_State(ParseState state);
    std::shared_ptr<HttpRequest> Get_request();
    std::shared_ptr<HttpRespone> Get_respone();
    // ParseState Get_State();
    ~HttpContext();
};

}
} // namespace Wasi::Http

#endif // !HTTP_CONTEXT_H_