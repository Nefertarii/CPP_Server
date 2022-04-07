#ifndef HTTP_CONTEXT_H_
#define HTTP_CONTEXT_H_

namespace Wasi {
namespace Http {

class HttpContext {
private:
    enum class ParseState {
        LINE,
        HEAD,
        BODY,
        ALLDONE,
    };
    
public:
    HttpContext();
    void Process_request();
    
    ~HttpContext();
};
}
} // namespace Wasi::Http

#endif // !HTTP_CONTEXT_H_