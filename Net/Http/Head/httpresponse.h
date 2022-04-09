#ifndef HTTP_RESPONE_H_
#define HTTP_RESPONE_H_

#include "httpenum.h"

namespace Wasi {
namespace Http {

class HttpRespone {
private:
    ResponeHead respone_head;
    std::string respone_string;
    // std::string respone_file;

public:
    HttpRespone(std::string http_request);
    void Swap(HttpRespone& other);
    void Set_code_num(std::string code_num);
    void Set_server_name(std::string server_name);
    void Set_connection_type(std::string connection_type);
    void Set_content_type(std::string content_type);
    void Set_content_length(std::string content_length);
    void Set_last_modified(std::string last_modified);
    HttpHead Get_respone_head();
    ~HttpRespone();
};

}
} // namespace Wasi::Http

#endif // !HTTP_RESPONE_H_