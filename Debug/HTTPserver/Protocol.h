#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <string>
#include "../Important/Gtime.h"
#include "../Important/Httprocess/Httprocess.h"
#include "../Important/Httprocess/Httpconnect.h"
#include "../Important/Httprocess/Httprespone.h"

class Badrespone {
private:
    std::string responehead;
public:
    Badrespone(){};
    std::string Respone404();
    void Respone404(std::string *str);
    ~Badrespone(){};
};








std::string Badrespone::Respone404() {
    responehead.clear();
    responehead += "HTTP/1.1 404 Bad Request\r\n";
    responehead += "Constent_Charset:utf-8\r\n";
    responehead += "Content-Language:zh-CN\r\n";
    responehead += "Content-Length:0\r\n";
    responehead += Timer::Nowtime_str() + "\r\n";
    responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
    return responehead;
}

void Badrespone::Respone404(std::string *str) {
    str->clear();
    *str += "HTTP/1.1 404 Bad Request\r\n";
    *str += "Constent_Charset:utf-8\r\n";
    *str += "Content-Language:zh-CN\r\n";
    *str += "Content-Length:0\r\n";
    *str += Timer::Nowtime_str() + "\r\n";
    *str += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

#endif