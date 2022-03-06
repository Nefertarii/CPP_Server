#include "Head/buffer.h"
#include <algorithm>
#include <assert.h>

using namespace Wasi::Base;

const char Buffer::CRLF = "\r\n";
const size_t Buffer::initial_size = 1024;

Buffer::Buffer(size_t size) :
    buffer(initial_size),
    index(0),
    state(READER) {}

size_t Buffer::Find(char* str) const {
    auto it = std::search(buffer.begin(), buffer.end(), str);
    return std::distance(buffer.begin(), it);
}

size_t Buffer::Find(const size_t begin, char* str) const {
    auto it = std::search(buffer.begin() + begin, buffer.end(), str);
    return std::distance(buffer.begin(), it);
}

size_t Buffer::First_CRLF() const { return Find(CRLF); }

size_t Buffer::First_CRLF(const size_t begin) const { return Find(begin, CRLF); }

size_t Buffer::First_EOL() const { return Find("\n"); }

size_t Buffer::First_EOL(const size_t begin) const { return Find(begin, "\n"); }

void Buffer::Append(const std::string& str) { buffer += str; }

void Buffer::Append(const char* str, size_t len) { buffer.append(str, 0, len); }

void Buffer::Swap(Buffer& rhs) {
    buffer.swap(rhs.buffer);
    std::swap(state, rhs.state);
}

Buffer::BufState Buffer::Buffer_state() const { return state; }

Buffer Buffer::operator=(const Buffer& rhs) {
    buffer = rhs.buffer;
    state = rhs.state;
    return this;
}

Buffer Buffer::operator+=(const Buffer& rhs) {
    buffer += rhs.buffer;
    return this;
}

ssize_t Buffer::Read_fd(int fd, int* tmp_errno) {

}

ssize_t Buffer::Send_fd(int fd, int* tmp_errno) {

}

std::string Buffer::Content() { return buffer; }