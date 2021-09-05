#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "Httprocess/Httprocess.h"
#include "Httprocess/Httpconnect.h"
#include "Httprocess/Httprespone.h"


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