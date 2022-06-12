#include <iostream>
#include <map>
#include <string>
#include <vector>

class NetJson {
private:
    // std::map<std::string> array;
    std::string json;

public:
    NetJson(){};
    std::string& operator[](std::string key) {
        return json;
    }
    void operator=(std::string value) {
    }
    void print() { std::cout << json << "\n"; }
    ~NetJson(){};
};

using namespace std;

void func1() {
    NetJson json;
    json["111"];
    json.print();
    json["333"] = "555";
    json.print();
}

void func2() {
    std::string user_image = "image";
    std::string user_alias = "alias";
    //
    std::string str = "{\"state\":\"success\",";
    str += "\"AccountImage\":\"";
    str += user_image;
    str += "\",";
    str += "\"AccountAlias\":\"";
    str += user_alias;
    str += "\"}";

    cout << str << "\n";
}

int main() {
    func2();
}