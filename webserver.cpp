#include "Include/Web/Http/httpprocess.h"
#include "Include/Net/tcpconnection.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include "Include/Web/Http/httpserver.h"
#include "Include/Header/filehandler.h"
#include "Include/Header/thread.h"
#include "Include/Timer/clock.h"
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

std::map<std::string, std::string> ReadSetting(std::string file_path) {
    head::FileHandler file;
    std::map<std::string, std::string> setting;
    setting.clear();
    if (file.Open(file_path)) {
        std::string setting_str;
        file.Read(setting_str);
        size_t start_pos = 0;
        std::string stop_str = "\n";
        while (start_pos != std::string::npos && start_pos < setting_str.length()) {
            size_t end_pos           = setting_str.find(stop_str, start_pos);
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
                start_pos = end_pos + stop_str.size();
            }
        }
    }
    return setting;
}

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

loop::EventLoop* global_loop = nullptr;
bool running                 = false;

void WebServer() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string path_str       = path;
    // test line
    // path_str                                   = "/home/ubuntu/Server";
    std::string setting_file = path_str + "/server_setting.txt";
    LOG_INFO("server start in dir:" + path_str);
    LOG_INFO("reading setting file:" + setting_file);
    std::map<std::string, std::string> setting = ReadSetting(setting_file);
    if (setting.empty()) {
        LOG_FATAL("please check file, setting file can't find");
        std::cout << "Server control:";
        running = false;
    } else {
        loop::EventLoop loop;
        HttpServer server(&loop, setting);
        loop::EventLoop server_loop;
        global_loop = &server_loop;
        web::HttpServer webserver(&server_loop, setting);
        webserver.Start();
        LOG_INFO("server start");
        running = true;
        server_loop.Loop();
    }
}

bool WebserverStart() {
    if (running == false) {
        LOG_INFO("Ready to start server");
        head::Thread server_thread(WebServer, "server thread");
        server_thread.Start();
        return true;
    } else {
        LOG_INFO("Server is running");
        return false;
    }
}

bool WebserverStop() {
    if (running == false) {
        LOG_INFO("Server is not running");
        return false;
    } else {
        running = false;
        global_loop->Quit();
        LOG_INFO("Server is stop now");
        return true;
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
            std::this_thread::sleep_for(timer::Timer_Sec(1));
            std::cout << "Reboot in 2 seconds.\n";
            std::this_thread::sleep_for(timer::Timer_Sec(1));
            std::cout << "Reboot in 1 seconds.\n";
            std::this_thread::sleep_for(timer::Timer_Sec(1));
            std::cout << "Rebooting...\n";
            WebserverStart();
        } else if (command == "quit") {
            if (WebserverStop() == true) {
                std::cout << "Wait saving data.";
                for (int k = 0; k <= 10; k++) {
                    std::this_thread::sleep_for(timer::Timer_Ms(300));
                    std::cout << ".";
                }
                std::cout << std::endl;
            }
            std::cout << "Server control quit.\n";
            break;
        } else {
            std::cout << "Undefined command: " << command << ".  Try 'help'.\n";
        }
    }
}

int main() {
    log::Logger::Init();
    ControlFunc();
}