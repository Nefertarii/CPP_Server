#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_

#include <vector>
#include <algorithm>
#include <assert.h>
#include <cstring>

namespace Wasi {
    namespace Base {
        class Buffer {
        private:
            static const char CRLF[];
            std::vector<char> buffer;
            size_t reader_index;
            size_t writer_index;
            char* Begin();
            const char* Begin() const;
            void Make_space(size_t len);
        public:
            static const size_t cheap_prepend = 8;
            static const size_t initial_size = 1024;
            explicit Buffer(size_t size = initial_size);
            //...
        };
    }
}

#endif