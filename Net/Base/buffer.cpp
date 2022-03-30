#include "Head/buffer.h"
#include "../../Log/Head/logging.h"
#include <algorithm>
#include <assert.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace Wasi;
using namespace Wasi::Base;

const char Buffer::CRLF[] = "\r\n";
// const size_t Buffer::initial_size = 1024;

Buffer::Buffer() :
    index(0),
    state(READ) {}

Buffer::Buffer(BufferState state_) :
    index(0),
    state(state_) {}

Buffer::Buffer(std::string str) :
    index(0),
    buffer(str),
    state(WRITE) {}

size_t Buffer::Find(const char* str) { return buffer.find(str); }

size_t Buffer::Find(const char* str, size_t begin) { return buffer.find(str, begin); }

size_t Buffer::First_CRLF() { return Find(CRLF); }

size_t Buffer::First_CRLF(size_t begin) { return Find(CRLF, begin); }

size_t Buffer::First_EOL() { return Find("\n"); }

size_t Buffer::First_EOL(size_t begin) { return Find("\n", begin); }

size_t Buffer::Index() const { return index; }

size_t Buffer::Size() const { return buffer.length(); }

size_t Buffer::Remaining() const { return buffer.length() - index; }

Buffer::BufferState Buffer::State() const { return state; }

void Buffer::Add_index(size_t num) {
    size_t remaining = buffer.length() - index;
    assert(num <= remaining);
    if (num < remaining) {
        index += num;
    } else {
        Init();
    }
}

void Buffer::Append(const std::string& str) { buffer += str; }

void Buffer::Append(const char* str, size_t len) { buffer.append(str, len); }

void Buffer::Swap(Buffer& rhs) { buffer.swap(rhs.buffer); }

void Buffer::Init() {
    index = 0;
    buffer.clear();
}

Buffer Buffer::operator+(const Buffer& rhs) {
    buffer += rhs.buffer;
    return *this;
}

Buffer& Buffer::operator=(const Buffer& rhs) {
    buffer = rhs.buffer;
    index  = rhs.index;
    return *this;
}

Buffer& Buffer::operator=(const std::string& str) {
    buffer = str;
    return *this;
}

Buffer& Buffer::operator+=(const Buffer& rhs) {
    buffer += rhs.buffer;
    return *this;
}

ssize_t Buffer::Read_fd(int fd, int* tmp_errno) {
    if (state == READ) {
        iovec vec;
        char extrabuf[65536];
        vec.iov_base       = extrabuf;
        vec.iov_len        = sizeof(extrabuf);
        const int iovcnt   = 1;
        const ssize_t read = readv(fd, &vec, iovcnt);
        if (read < 0) {
            *tmp_errno = errno;
            return 0;
        }
        Append(extrabuf, read);
        return read;
    }
    LOG_INFO("Buffer::Read_fd() Buffer state is WRITE.");
    return 0;
}

std::string Buffer::Content() { return buffer; }

Buffer::~Buffer() {}