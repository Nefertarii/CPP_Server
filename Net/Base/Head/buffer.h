#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_

#include <vector>
#include <string>

namespace Wasi {
    namespace Base {
        class Buffer {
        private:
            static const char CRLF[];
            size_t index; //tcp send index
            std::string buffer;
        public:
            static const size_t initial_size;
            explicit Buffer(size_t size = initial_size);
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
            void Add_index(int num);
            void Append(const std::string& str);
            void Append(const char* str, size_t len);
            void Swap(Buffer& rhs);
            void Init();
            Buffer operator+(const Buffer&);
            Buffer& operator=(const Buffer&);
            Buffer& operator=(const std::string& str);
            Buffer& operator+=(const Buffer&);
            ssize_t Read_fd(int fd, int* tmp_errno);
            //ssize_t Send_fd(int fd, int* tmp_errno);
            std::string Content();
            ~Buffer();
        };
    }
}

#endif