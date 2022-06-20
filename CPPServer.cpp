#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Http/httpserver.h>

using namespace Wasi;
using namespace std;

void Webserver() {
    // Log::Logging::Change_default_logger(std::make_shared<Log::FileSink>("test.log"));
    Poll::EventLoop server_loop;
    Sockets::InetAddress linsten("127.0.0.1", 8000);
    Http::HttpServer webserver(&server_loop, linsten, "webserver");
    // webserver.Set_thread_num(1);
    webserver.Start();
    server_loop.Loop();
}

int main() {
    Webserver();
}