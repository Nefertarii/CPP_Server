#ifndef HTTP_RESPONE_H_
#define HTTP_RESPONE_H_

#include "httpenum.h"

namespace Wasi {
namespace Http {

class HttpRespone {
public:
    ResponeHead respone_head;
    std::string respone_body;
    std::string respone_file;
    HttpRespone();
    ~HttpRespone();
};

}
} // namespace Wasi::Http

#endif // !HTTP_RESPONE_H_