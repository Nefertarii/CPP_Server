#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

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
struct HttpHead {
    std::string content_type;
    std::string content_length;
    std::string content_encoding;
    std::string cache_control;
    std::string date;
    std::string etag;
    std::string location;
    std::string last_modified;
    std::string server;
};

inline std::string Filename(std::string name) {
    return std::string(name.begin() + name.find_last_of('/'), name.end());
}

int flag;           // 11111
Method method;      // 00001
Version version;    // 00010
std::string body;   // 00100
std::string target; // 01000
std::string path;   // 10000

class A {
private:
    int T;

public:
    int print() { return T; }
};

void func1(A& a) {
    a.T = 2;
}

int main() {
    // clang-format off
    //std::string message("POST /path/path2/home.html HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nIf-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT\r\n\r\nbody123456" );
    //Parse(&message);
    //  clang-format on
    //  cout << message;
    A a;
    func1(a);
    cout << a.print();
}
