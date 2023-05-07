#include "Include/Web/Http/httpprocess.h"
#include "Include/Net/tcpconnection.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Web/Http/httpserver.h"
#include "Include/Header/filehandler.h"
#include "Include/Header/thread.h"
#include <iostream>
#include <filesystem>

using namespace wasi;
using namespace wasi::web;

// void httpserver_t() {
//     loop::EventLoop loop;
//     net::InetAddress listen("0.0.0.0", 8000);
//     HttpServer server(&loop, listen, "server#1");
//     server.SetThreadNum(4);
//     server.Start();
//     // loop.RunAfter(60.0, std::bind(&loop::EventLoop::Quit, &loop));
//     loop.Loop();
// }

std::map<std::string, std::string> Read_setting(std::string file_path) {
    head::FileHandler file;
    std::map<std::string, std::string> setting;
    setting.clear();
    if (file.Open(file_path)) {
        std::string setting_str;
        file.Read(setting_str);
        size_t start_pos = 0;
        while (start_pos != std::string::npos && start_pos < setting_str.length()) {
            size_t end_pos           = setting_str.find("\r\n", start_pos);
            std::string setting_line = setting_str.substr(start_pos, end_pos - start_pos);
            if (setting_line[0] != '#') {
                size_t header_delim_pos = setting_line.find(": ");
                if (header_delim_pos != std::string::npos) {
                    std::string setting_name  = setting_line.substr(0, header_delim_pos);
                    std::string setting_value = setting_line.substr(header_delim_pos + 2);
                    setting[setting_name]     = setting_value;
                }
            }
            if (end_pos == std::string::npos) {
                break;
            } else {
                start_pos = end_pos + 2;
            }
        }
    } else {
        std::cout << "can't find setting file\n";
    }
    return setting;
}

void httpserver_t2() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string path_str       = path;
    //
    path_str = "/home/ubuntu/Server";
    //
    std::map<std::string, std::string> setting = Read_setting(path_str + "/Setting/server_setting");
    if (!setting.empty()) {
        for (auto i : setting) {
            std::cout << i.first << ":" << i.second << "\n";
        }
    }
    std::cout << "\n";

    loop::EventLoop loop;
    HttpServer server(&loop, setting);

    server.Start();
    // loop.RunAfter(60.0, std::bind(&loop::EventLoop::Quit, &loop));
    loop.Loop();
}

loop::EventLoop* global_loop = nullptr;
bool running                 = false;

std::string GetInput() {
    std::string input;
    std::cin >> input;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Invalid. Please reenter: ";
        std::cin >> input;
    }
    return input;
}

void WebServer() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string path_str       = path;
    // test ok
    path_str                                   = "/home/ubuntu/Server";
    std::map<std::string, std::string> setting = Read_setting(path_str + "/Setting/server_setting");
    loop::EventLoop loop;
    HttpServer server(&loop, setting);

    loop::EventLoop server_loop;
    global_loop = &server_loop;
    web::HttpServer webserver(&server_loop, setting);
    // webserver.Set_thread_num(1);
    webserver.Start();
    LOG_INFO("server start");
    server_loop.Loop();
}

bool WebserverStop() {
    if (running == false) {
        LOG_INFO("Server is not running");
        return false;
    } else {
        global_loop->Quit();
        running = false;
        LOG_INFO("Server is stop now");
        return true;
    }
}

bool WebserverStart() {
    if (running == false) {
        LOG_INFO("Ready to start server");
        running = true;
        head::Thread server_thread(WebServer, "server thread");
        server_thread.Start();
        return true;
    } else {
        LOG_INFO("Server is running");
        return false;
    }
}

void ControlFunc() {
    std::string command;
    int times = 0;
    std::cout << "Server control(type 'help' see command list): ";
    while (1) {
        if (times++) {
            std::cout << "Server control:";
        }
        command = GetInput();
        if (command == "help") {
            std::cout << "'start' : start server\n";
            std::cout << "'stop'  : stop server\n";
            std::cout << "'reboot': reboot server\n";
            std::cout << "'quit'  : quit program\n";
        } else if (command == "list") {
            // cout list info
        } else if (command == "stop") {
            WebserverStop();
        } else if (command == "start") {
            WebserverStart();
        } else if (command == "reboot") {
            WebserverStop();
            std::cout << "Reboot in 3 seconds.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "Rebooting...\n";
            WebserverStart();
        } else if (command == "quit") {
            if (WebserverStop() == true) {
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
    log::Logger::Init();
    /// httpserver_t();
    //httpserver_t2();
    ControlFunc();
}