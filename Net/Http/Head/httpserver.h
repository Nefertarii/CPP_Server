#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

namespace Wasi {
namespace Http {

class HttpServer {
private:
    void Connection();
    void Message();
    void Request();
};
}
} // namespace Wasi::Http

#endif // !HTTP_SERVER_H_