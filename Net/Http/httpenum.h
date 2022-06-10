#ifndef HTTP_ENUM_H_
#define HTTP_ENUM_H_

#include <string>

namespace Wasi {
namespace Http {

inline static std::string html_dir       = "/home/nefertarii/vscode/HTML";
inline static std::string account_dir    = "/home/nefertarii/vscode/CPP_Server/userinfo/account.txt";
inline static std::string user_image_dir = "/home/nefertarii/vscode/CPP_Server/userinfo/";

enum class Method {
    INVAILD,
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};

enum class HttpVersion {
    UNKNOWN,
    HTTP10,
    HTTP11,
    HTTP20
};

enum class HttpCode {
    CODE200, // 200 OK
    CODE301, // 301 Moved Permanently
    CODE304, // 304 Not Modified
    CODE400, // 400 Bad Request
    CODE403, // 403 Forbidden
    CODE404, // 404 Not Found
    CODE411, // 411 Length Required
    CODE500, // 500 Internal Server Error
    CODE501  // 501 Method Not Implemented
};

enum class ConnectionType {
    UNKNOWN,
    KEEPALIVE // Keep-Alive
};

enum class FileType {
    HTML,       // text/html
    CSS,        // text/css
    JAVASCRIPT, // text/javascript
    JSON,       // application/json
    PNG,        // image/png
    SVG,        // image/svg+xml
    ICON,       // image/x-icon
    PLAIN       // text/plain
};

enum class HttpCharset {
    UNKNOWN,
    UTF8
};

enum class PostMethod {
    UNKNOWN,
    LOGIN,
    RESET,
    REGISTER,
};

struct ResponeHead {
    HttpVersion http_version;  //
    HttpCode code_num;         // HTTP/1.1 (...) (...)
    std::string server;        // Server: (...)
    ConnectionType connection; // Connection: (...)
    int keep_alive_timeout;    //
    int keep_alive_max;        // Keep-Alive: timeout=(...), max=(...)
    FileType content_type;     //
    HttpCharset charset;       // Content-Type: (...); charset=(....)
    long content_length;       // Content-Length: (...)
    time_t last_modified;      // Last-Modified: (...) Week, Day Month Year HH:MM:SS GMT
    time_t date;               // date = Week, Day Month Year HH:MM:SS GMT
    // std::string etag;
    // std::string content_encoding;
    // std::string cache_control;
    ResponeHead() :
        http_version(HttpVersion::UNKNOWN),
        code_num(HttpCode::CODE404),
        server("C++_Http_Server"),
        connection(ConnectionType::UNKNOWN),
        keep_alive_timeout(0),
        keep_alive_max(0),
        content_type(FileType::PLAIN),
        charset(),
        content_length(0),
        last_modified(0),
        date(0) {}
};
}
} // namespace Wasi::Http

#endif