#include <Base/Poll/eventloop.h>
#include <Base/Thread/thread.h>
#include <Log/logging.h>
#include <Net/Http/httpserver.h>
#include <iostream>

using namespace std;
using namespace Wasi;

Poll::EventLoop* global_loop = nullptr;

std::string Get_input() {
    std::string tmp;
    cin >> tmp;
    while (cin.fail()) {
        cin.clear();
        cin.ignore();
        cout << "Invalid. Please reenter: ";
        cin >> tmp;
    }
    return tmp;
}

void Webserver() {
    // Log::Logging::Change_default_logger(make_shared<Log::FileSink>("test.log"));
    Poll::EventLoop server_loop;
    global_loop = &server_loop;
    Sockets::InetAddress linsten("127.0.0.1", 8000);
    Http::HttpServer webserver(&server_loop, linsten, "webserver");
    // webserver.Set_thread_num(1);
    webserver.Start();
    cout << "server start.\n";
    server_loop.Loop();
}

void Control_func() {
    std::string command;
    bool running = false;

    cout << "Server control(type 'help' see command list): ";
    while (1) {
        command = Get_input();
        if (command == "help") {
            cout << "'start' : start server\n";
            cout << "'stop'  : stop server\n";
            cout << "'reboot': reboot server\n";
            cout << "'quit'  : quit program\n";
        } else if (command == "list") {
            // cout list info
        } else if (command == "stop") {
            global_loop->Quit();
            running = false;
        } else if (command == "start") {
            if (running == false) {
                cout << "start run server\n";
                running = true;
                Base::Thread server_thread(Webserver, "server thread");
                server_thread.Start();
            } else {
                cout << "server is running\n";
            }
        } else if (command == "reboot") {
            global_loop->Quit();
            running = false;
            cout << "Reboot in 3 seconds.\n";
            this_thread::sleep_for(chrono::seconds(3));
            cout << "Rebooting...";
            if (running == false) {
                cout << "start run server\n";
                Base::Thread server_thread(Webserver, "server thread");
                server_thread.Start();
            } else {
                cout << "server is running\n";
            }
        } else if (command == "quit") {
            global_loop->Quit();
            running = false;
            cout << "Wait saving data.\n";
            this_thread::sleep_for(chrono::seconds(3));
            cout << "Server quit.\n";
            break;
        } else {
            cout << "Undefined command: " << command << ".  Try 'help'.\n";
        }
    }
}

int main() {
    Control_func();
    return 0;
}