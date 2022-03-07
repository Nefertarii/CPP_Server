#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_

#include <vector>
#include <string>

namespace Wasi {
    namespace Base {
        enum BufState {
            READER,
            WRITER
        };
        
        class Buffer {
        private:
            static const char CRLF[];
            std::string buffer;
            size_t index; //tcp send index
            BufState state;
        public:
            static const size_t initial_size;
            explicit Buffer(size_t size = initial_size);
            size_t Find(const char* str);
            size_t Find(const char* str, size_t begin);
            size_t First_CRLF();
            size_t First_CRLF(size_t begin);
            size_t First_EOL();
            size_t First_EOL(size_t begin);
            BufState Buffer_state() const;
            void Append(const std::string& str);
            void Append(const char* str, size_t len);
            void Swap(Buffer& rhs);
            Buffer operator+(const Buffer&);
            Buffer& operator=(const Buffer&);
            Buffer& operator+=(const Buffer&);
            ssize_t Read_fd(int fd, int* tmp_errno);
            //ssize_t Send_fd(int fd, int* tmp_errno);
            std::string Content();
            ~Buffer();
        };
    }
}

#endif