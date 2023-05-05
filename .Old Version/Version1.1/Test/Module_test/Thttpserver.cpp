#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Http/httpserver.h>

using namespace Wasi;
using namespace std;

void Thttpserver() {
    // Log::Logging::Change_default_logger(std::make_shared<Log::FileSink>("test.log"));
    Poll::EventLoop server_loop;
    Sockets::InetAddress linsten("127.0.0.1", 8000);
    Http::HttpServer httpserver(&server_loop, linsten, "httpserver");
    httpserver.Start();
    server_loop.Loop();
}

int main() {
    Thttpserver();
}