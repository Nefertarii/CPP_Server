#include "Head/buffer.h"
#include <algorithm>
#include <assert.h>
#include <sys/uio.h>

using namespace Wasi::Base;

const char Buffer::CRLF[] = "\r\n";
const size_t Buffer::initial_size = 1024;

Buffer::Buffer(size_t size) :
    index(0),
    state(READER) {}

//            size_t Find(char* str) const;
//            size_t Find(char* str, const size_t begin) const;

size_t Buffer::Find(const char* str) { return index = buffer.find(str); }

size_t Buffer::Find(const char* str, size_t begin) { return index = buffer.find(str, begin); }

size_t Buffer::First_CRLF() { return Find(CRLF); }

size_t Buffer::First_CRLF(size_t begin) { return Find(CRLF, begin); }

size_t Buffer::First_EOL() { return Find("\n"); }

size_t Buffer::First_EOL(size_t begin) { return Find("\n", begin); }

void Buffer::Append(const std::string& str) { buffer += str; }

void Buffer::Append(const char* str, size_t len) { buffer.append(str, len); }

void Buffer::Swap(Buffer& rhs) {
    buffer.swap(rhs.buffer);
    std::swap(state, rhs.state);
}

BufState Buffer::Buffer_state() const { return state; }

Buffer Buffer::operator+(const Buffer& rhs) {
    buffer += rhs.buffer;
    return *this;
}

Buffer& Buffer::operator=(const Buffer& rhs) {
    buffer = rhs.buffer;
    state = rhs.state;
    return *this;
}

Buffer& Buffer::operator+=(const Buffer& rhs) {
    buffer += rhs.buffer;
    state = rhs.state;
    return *this;
}

ssize_t Buffer::Read_fd(int fd, int* tmp_errno) {
    iovec vec;
    char extrabuf[65536];
    vec.iov_base = extrabuf;
    vec.iov_len = sizeof(extrabuf);
    const int iovcnt = 1;
    const int read = readv(fd, &vec, iovcnt);
    if (read < 0) {
        *tmp_errno = errno;
        return 0;
    }
    Append(extrabuf, read);
    state = READER;
    return read;
}

//ssize_t Buffer::Send_fd(int fd, int* tmp_errno) {}

std::string Buffer::Content() { return buffer; }

Buffer::~Buffer() {}