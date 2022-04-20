#include <any>
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

class Test {
public:
    int i = 0;
    Test(int num) :
        i(num) { cout << "test create " << i << "\n"; }
    ~Test() { cout << "test earse " << i << "\n"; }
};

int main() {
    // clang-format off
    //std::string message("POST /path/path2/home.html HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nIf-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT\r\n\r\nbody123456" );
    //Parse(&message);
    //  clang-format on
    //  cout << message;
    // shared_ptr<void> ptr_1 = nullptr;
    // shared_ptr<Test> ptr_2 = make_shared<Test>(5);
    // 
    // ptr_1 = static_pointer_cast<Test>(ptr_2);
    // Test* ptr_1_data = (Test*)ptr_1.get();

    // cout<<ptr_1_data->i<<"\n";
    // cout<<ptr_2.use_count()<<"\n";
    // cout<<ptr_2->i<<"\n";

        std::cout << std::boolalpha;
 
    // any type
    std::any a = 1;
    std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
    a = 3.14;
    std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
    a = true;
    std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';
 
    // bad cast
    try{
        a = 1;
        std::cout << std::any_cast<float>(a) << '\n';
    }
    catch (const std::bad_any_cast& e){
        std::cout << e.what() << '\n';
    }
 
    // has value
    a = 2;
    if (a.has_value()){
        std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
    }
 
    // reset
    a.reset();
    if (!a.has_value()) {
        std::cout << "no value\n";
    }
 
    // pointer to contained data
    a = 3;
    int* i = std::any_cast<int>(&a);
    std::cout << *i << "\n";

    any* a_p;


}

