#ifndef HTTP_RESPONE_H_
#define HTTP_RESPONE_H_

#include "httpenum.h"

namespace Wasi {
namespace Http {

class HttpRespone {
private:
    Method method;
    Version version;
    HttpHead respone_head;

public:
    HttpRespone(std::string http_request);
    void Swap(HttpRespone& other);
    ~HttpRespone();
};

}
} // namespace Wasi::Http

#endif // !HTTP_RESPONE_H_