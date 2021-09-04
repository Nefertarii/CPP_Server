#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "Httprocess/Httprocess.h"

class Badrespone {
private:
    std::string responehead;
public:
    Badrespone(){};
    std::string Respone200();
    void Respone200(std::string *str);
    ~Badrespone(){};
};

#endif