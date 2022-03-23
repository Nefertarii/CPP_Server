#ifndef LOG_CONFIG_H_
#define LOG_CONFIG_H_

#include <map>
#include <string>
#include <vector>

namespace Wasi {
namespace Log {

enum class LogLevel;

class LogConfig {
public:
    enum class LogOutput {
        OUTINIT = 0, // default = consoles
        OUTCONSOLES,
        OUTFILE,
        OUTNETWORK
    };
    LogOutput output;
    std::string filedir;
    std::string netaddr;
    std::vector<LogLevel> filter_in;
    std::vector<LogLevel> filter_out;

    LogConfig() :
        output(LogOutput::OUTINIT),
        filedir("log1.txt"),
        netaddr("127.0.0.1:9021") {
        filter_in.clear();
        filter_out.clear();
    }
    /*
    void Set_output(LogOutput location);
    void Set_filedir(std::string filedir_);
    void Set_netaddr(std::string netaddr_);
    void Set_consoles_color(LogLevel level, std::string color);
    void Add_filter_in(LogLevel level);
    void Add_filter_out(LogLevel level);
    void Add_output_immed(LogLevel level);
    void Remove_filter_in(LogLevel level);
    void Remove_filter_out(LogLevel level);
    void Remove_output_immed(LogLevel level);
    LogOutput Get_output() const;
    std::string Get_filedir() const;
    std::string Get_netaddr() const;
    std::string Get_consoles_color(LogLevel level) const;
    bool Find_filter_in(LogLevel level) const;
    bool Find_filter_out(LogLevel level) const;
    bool Find_output_immed(LogLevel level) const;
    ~LogConfig();
    */
};

}
} // namespace Wasi::Log
