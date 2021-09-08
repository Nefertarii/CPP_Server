#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <string>
#include "../Important/Gtime.h"

class Badrespone {
private:
    std::string responehead;
public:
    Badrespone(){};
    std::string Respone404();
    void Respone404(std::string *str);
    ~Badrespone(){};
};

#endif