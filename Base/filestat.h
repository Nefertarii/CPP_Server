#ifndef BASE_FILESTAT_H_
#define BASE_FILESTAT_H_

#include <string>

namespace Wasi {
namespace Base {

class FileStat {
public:
    std::string file_name;
    int filefd;
    long file_offset;
    long file_size;
    FileStat();
    void Init();
    long Remaning();
};

}
}; // namespace Wasi::Base

#endif // !BASE_FILESTAT_H_