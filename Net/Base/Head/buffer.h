#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_

#include <string>
#include <vector>

namespace Wasi {
namespace Base {

class Buffer {
public:
    enum BufferState {
        READ,
        WRITE
    };

private:
    static const char CRLF[];
    size_t index; // tcp send index
    std::string buffer;
    BufferState const state;

public:
    static const size_t initial_size;
    explicit Buffer();
    Buffer(BufferState state_);
    Buffer(std::string str);
    size_t Find(const char* str);
    size_t Find(const char* str, size_t begin);
    size_t First_CRLF();
    size_t First_CRLF(size_t begin);
    size_t First_EOL();
    size_t First_EOL(size_t begin);
    size_t Index() const;
    size_t Size() const;
    size_t Remaining() const;
    BufferState State() const;
    void Add_index(size_t num);
    void Append(const std::string& str);
    void Append(const char* str, size_t len);
    void Swap(Buffer& rhs);
    void Init();
    Buffer operator+(const Buffer&);
    Buffer& operator=(const Buffer&);
    Buffer& operator=(const std::string& str);
    Buffer& operator+=(const Buffer&);
    ssize_t Read_fd(int fd, int* tmp_errno);
    std::string Content();
    ~Buffer();
};

}
} // namespace Wasi::Base

#endif // !BASE_BUFFER_H_