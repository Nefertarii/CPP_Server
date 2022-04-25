#ifndef HTTP_ENUM_H_
#define HTTP_ENUM_H_

#include <string>

namespace Wasi {
namespace Http {

enum class Method {
    INVAILD,
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};

enum class Version {
    UNKNOWN,
    HTTP10,
    HTTP11,
    HTTP20
};

enum class HttpCode {
    CODE200 = 200, // 200 OK
    CODE301 = 301, // 301 Moved Permanently
    CODE304 = 304, // 304 Not Modified
    CODE400 = 400, // 400 Bad Request
    CODE403 = 403, // 403 Forbidden
    CODE404 = 404, // 404 Not Found
    CODE411 = 411, // 411 Length Required
    CODE500 = 500, // 500 Internal Server Error
    CODE501 = 501, // 501 Method Not Implemented
};

struct ResponeHead {
    std::string code_num;           // HTTP/1.1 (...) (...)
    std::string server;             // Server: (...)
    std::string connection;         // Connection: (...)
    std::string keep_alive_timeout; //
    std::string keep_alive_max;     // Keep-Alive: (timeout=(...), max=(...))
    std::string content_type;       //
    std::string charset;            // Content-Type: (...); charset=(....)
    std::string content_length;     // Content_length: (...)
    std::string last_modified;      // Last-Modified: (...)
    std::string date;               // date = Week, Day Month Year HH:MM:SS GMT
    // std::string etag;
    // std::string content_encoding;
    // std::string cache_control;
};

}
} // namespace Wasi::Http

#endif