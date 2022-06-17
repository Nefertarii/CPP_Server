#include <Base/Poll/eventloop.h>
#include <Log/logging.h>
#include <Net/Tcp/tcpclient.h>

using namespace Wasi;
using namespace std;

void func1() {
    Poll::EventLoop main_loop;
    Sockets::InetAddress cli_address("159.75.51.91", 80);
    Server::TcpClient client(&main_loop, cli_address, "client");
    client.Connect();
    main_loop.Loop();
}

int main() {
    func1();
}