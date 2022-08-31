#include <Base/Poll/eventloop.h>
#include <Base/Thread/thread.h>
#include <Log/logging.h>
#include <Net/Http/httpserver.h>
#include <iostream>

using namespace Wasi;

Poll::EventLoop* global_loop = nullptr;
bool running                 = false;

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
    Log::Logging::Change_default_logger(std::make_shared<Log::FileSink>("test.log"));
    Poll::EventLoop server_loop;
    global_loop = &server_loop;
    Sockets::InetAddress linsten("0.0.0.0", 8000);
    Http::HttpServer webserver(&server_loop, linsten, "webserver");
    // webserver.Set_thread_num(1);
    webserver.Start();
    std::cout << "server start.\n";
    server_loop.Loop();
}

bool Webserver_stop() {
    if (running == false) {
        std::cout << "Server is not running\n";
        return false;
    } else {
        global_loop->Quit();
        running = false;
        std::cout << "Server is stop now\n";
        return true;
    }
}

bool Webserver_start() {
    if (running == false) {
        std::cout << "Ready to start server\n";
        running = true;
        Base::Thread server_thread(Webserver, "server thread");
        server_thread.Start();
        return true;
    } else {
        std::cout << "Server is running\n";
        return false;
    }
}

void Control_func() {
    std::string command;
    int times = 0;
    std::cout << "Server control(type 'help' see command list): ";
    while (1) {
        if (times++) {
            std::cout << "Server control:";
        }
        command = Get_input();
        if (command == "help") {
            std::cout << "'start' : start server\n";
            std::cout << "'stop'  : stop server\n";
            std::cout << "'reboot': reboot server\n";
            std::cout << "'quit'  : quit program\n";
        } else if (command == "list") {
            // cout list info
        } else if (command == "stop") {
            Webserver_stop();
        } else if (command == "start") {
            Webserver_start();
        } else if (command == "reboot") {
            Webserver_stop();
            std::cout << "Reboot in 3 seconds.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "Rebooting...\n";
            Webserver_start();
        } else if (command == "quit") {
            if (Webserver_stop() == true) {
                std::cout << "Wait saving data.\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
            std::cout << "Server quit.\n";
            break;
        } else {
            std::cout << "Undefined command: " << command << ".  Try 'help'.\n";
        }
    }
}

int main() {
    Control_func();
    return 0;
}