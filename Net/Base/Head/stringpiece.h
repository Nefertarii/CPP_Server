#ifndef BASE_STRINGPIECE_H_
#define BASE_STRINGPIECE_H_

#include <string>

namespace Wasi {
    namespace Base {
        class StringPiece {
        private:
            const char* ptr;
            int length;
        public:
            StringPiece();
            StringPiece(const char* str);
            StringPiece(string str);
        };
    }
}


#endif