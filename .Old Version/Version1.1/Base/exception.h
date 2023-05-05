#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <cstring>
#include <string>
#include <string_view>

namespace Wasi {

class Exception {
private:
    std::string message;
    int tmp_errno;

public:
    Exception(std::string message_ = "unkonw exception.",
              int errno_           = 0) :
        message(message_),
        tmp_errno(errno_){};
    std::string What() const {
        if (tmp_errno != 0) {
            return message + " " + strerror(tmp_errno);
        }
        return message;
    }
};

} // namespace Wasi

#endif