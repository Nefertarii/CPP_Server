#ifndef LOG_CONFIG_H_
#define LOG_CONFIG_H_

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
    // enum class LogSet {};

private:
    LogOutput output;
    std::string filedir;
    std::string netaddr;
    std::string consoles_color;
    std::vector<LogLevel> filter_in;
    std::vector<LogLevel> filter_out;
    std::vector<LogLevel> output_immed;

public:
    LogConfig();
    void Set_output(LogOutput location);
    void Set_filedir(std::string filedir);
    void Set_netaddr(std::string netaddr);
    void Set_consoles_color(std::string color);
    void Add_filter_in(LogLevel level);
    void Add_filter_out(LogLevel level);
    void Add_output_immed(LogLevel level);
    void Remove_filter_in(LogLevel level);
    void Remove_filter_in(LogLevel level);
    void Remove_output_immed(LogLevel level);
    LogOutput Get_output() const;
    std::string Get_filedir() const;
    std::string Get_netaddr() const;
    std::vector<LogLevel> Get_filter_in() const;
    std::vector<LogLevel> Get_filter_out() const;
    std::vector<LogLevel> Get_output_immed() const;
    ~LogConfig();
};
}
} // namespace Wasi::Log
