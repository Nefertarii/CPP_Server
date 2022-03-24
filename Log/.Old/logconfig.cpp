/*
#include "Head/logconfig.h"
#include "Head/fontcolor.h"
#include "Head/loglevel.h"
#include <algorithm>

using namespace Wasi::Log;

LogConfig::LogConfig() :
}

void LogConfig::Set_output(LogOutput location) { output = location; }

void LogConfig::Set_filedir(std::string filedir_) { filedir = filedir_; }

void LogConfig::Set_netaddr(std::string netaddr_) { netaddr = netaddr_; }

void LogConfig::Set_consoles_color(LogLevel level, std::string color) {
    consoles_color[level] = color;
}

void LogConfig::Add_filter_in(LogLevel level) {
    if (std::find(filter_in.begin(), filter_in.end(), level) == filter_in.end()) {
        filter_in.push_back(level);
    }
}

void LogConfig::Add_filter_out(LogLevel level) {
    if (std::find(ilter_out.begin(), ilter_out.end(), level) == ilter_out.end()) {
        ilter_out.push_back(level);
    }
}

void LogConfig::Add_output_immed(LogLevel level) {
    if (std::find(output_immed.begin(), output_immed.end(), level) == output_immed.end()) {
        output_immed.push_back(level);
    }
}

void LogConfig::Remove_filter_in(LogLevel level) {
    auto earse_it = std::find(filter_in.begin(), filter_in.end(), level);
    if (earse_it != filter_in.end()) {
        filter_in.erase(earse_it);
    }
}

void LogConfig::Remove_filter_out(LogLevel level) {
    auto earse_it = std::find(filter_out.begin(), filter_out.end(), level);
    if (earse_it != filter_out.end()) {
        filter_out.erase(earse_it);
    }
}

void LogConfig::Remove_output_immed(LogLevel level) {
    auto earse_it = std::find(output_immed.begin(), output_immed.end(), level);
    if (earse_it != output_immed.end()) {
        output_immed.erase(earse_it);
    }
}

LogOutput LogConfig::Get_output() const { return output; }

std::string LogConfig::Get_filedir() const { return filedir; }

std::string LogConfig::Get_netaddr() const { return netaddr; }

std::string LogConfig::Get_consoles_color(LogLevel level) const {
    return consoles_color.find(level)->second;
}

bool LogConfig::Find_filter_in(LogLevel level) const {
    return std::find(filter_in.begin(), filter_in.end(), level) == filter_in.end();
}

bool LogConfig::Find_filter_out(LogLevel level) const {
    return std::find(filter_out.begin(), filter_out.end(), level) == filter_out.end();
}

bool LogConfig::Find_output_immed(LogLevel level) const {
    return std::find(output_immed.begin(), output_immed.end(), level) == output_immed.end();
}

LogConfig::~LogConfig();
*/