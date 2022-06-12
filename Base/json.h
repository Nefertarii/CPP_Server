#include <string>
#include <vector>

#include <iostream>

namespace Wasi {
namespace Base {

class NetJson {
private:
    std::vector<std::string> array;
    std::string json;

public:
    NetJson(){};
    void operator[](std::string key) {
        std::cout << "call []\n";
    }
    void operator=(std::string value) {
        std::cout << "call =\n";
    }
    ~NetJson(){};
};
}
} // namespace Wasi::Base