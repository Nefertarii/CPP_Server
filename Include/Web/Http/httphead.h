#ifndef WEB_HTTP_HTTPHEAD_H_
#define WEB_HTTP_HTTPHEAD_H_

#include <string>

namespace wasi {
namespace web {

enum class HttpMethod {
    kInvaild,
    kGet,
    kPost,
    kHead,
    kPut,
    kDelete,
    kTrace
};

enum class PostMethod {
    kInvaild,
    kLogin,
    kResetpwd,
    kRegsiter,
    kAccount
};

enum class HttpVersion {
    kUnknown,
    kHttp10,
    kHttp11,
    kHttp20,
};

enum class HttpStateCode {
    k200 = 200, // OK
    k301 = 301, // Moved Permanently
    k304 = 304, // Not Modified
    k400 = 400, // Bad Request
    k403 = 403, // Forbidden
    k404 = 404, // Not Found
    k411 = 411, // Length Required
    k500 = 500, // Internal Server Error
    k501 = 501  // Method Not Implemented
};

enum class FileType {
    kHtml,       // text/html
    kCss,        // text/css
    kJavascript, // text/javascript
    kJson,       // application/json
    kPng,        // image/png
    kSvg,        // image/svg+xml
    kIcon,       // image/x-icon
    kPlain       // text/plain
};

enum class HttpCharset {
    UNKNOWN,
    UTF8
};

enum class ResponeSendState {
    kWait,
    kHead,
    kBody,
    kDone,
    kError
};

class HttpRequestHead {
public:
    double keep_alive_timeout;
    double keep_alive_max;
    HttpMethod request_method;
    PostMethod post_method;
    HttpVersion version;
    HttpStateCode respone_code;
    std::string host;
    std::string accept; // acceptable response types
    std::string user_agent;
    std::string request_body;
    bool keep_alive;
    HttpRequestHead();
    HttpRequestHead(const HttpRequestHead& other);
    // find the assigned variable in "other" and replace "this"
    void SetExistFields(const HttpRequestHead& other);
    void Clear();
    HttpRequestHead& operator=(const HttpRequestHead& other);
};

class HttpResponeHead {
public:
    std::string respone_line;
    std::string connection;
    std::string connection_timeout;
    std::string content_length;
    std::string content_type;
    std::string last_modified;
    std::string charset;
    std::string date;
    std::string server_name;
    std::string respone_head;
    std::string respone_body;
    HttpResponeHead();
    HttpResponeHead(const HttpResponeHead& other);
    void HttpHeadMerge();
    // find the assigned variable in "other" and replace "this"
    void SetExistFields(const HttpResponeHead& other);
    void Clear();
    HttpResponeHead& operator=(const HttpResponeHead& other);
};

std::string TransHttpStateCode(HttpStateCode state_code);
std::string TransFileType(FileType str_file_type);
FileType TransFileType(std::string str_file_type);
PostMethod TransPostMethod(std::string str_post_type);
HttpMethod TransHttpMethod(std::string str_method);
std::string TransVersion(HttpVersion version);
HttpVersion TransVersion(std::string str_version);

}
} // namespace wasi::web

#endif