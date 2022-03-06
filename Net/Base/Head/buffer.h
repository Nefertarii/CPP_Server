#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_

#include <vector>
#include <string>

namespace Wasi {
    namespace Base {
        class Buffer {
        private:
            enum BufState {
                READER,
                WRITER
            };
            static const char CRLF[];
            std::string buffer;
            size_t index;
            BufState state;
        public:
            static const size_t initial_size;
            explicit Buffer(size_t size = initial_size);
            size_t Find(char* str) const;
            size_t Find(char* str, const size_t begin) const;
            size_t First_CRLF() const;
            size_t First_CRLF(const size_t begin) const;
            size_t First_EOL() const;
            size_t First_EOL(const size_t begin) const;
            BufState Buffer_state() const;
            void Append(const std::string& str);
            void Append(const char* str, size_t len);
            void Swap(Buffer& rhs);
            Buffer operator=(const Buffer&);
            Buffer operator+(const Buffer&);
            Buffer operator+=(const Buffer&);
            ssize_t Read_fd(int fd, int* tmp_errno);
            ssize_t Send_fd(int fd, int* tmp_errno);
            std::string Content();
            ~Buffer();
        };
    }
}

#endif