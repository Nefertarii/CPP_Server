#include "Protocol.h"

std::string Badrespone::Respone200() {
    responehead.clear();
    responehead += "HTTP/1.1 200 Bad Request\r\n";
    responehead += "Constent_Charset:utf-8\r\n";
    responehead += "Content-Language:zh-CN\r\n";
    responehead += "Content-Length:0\r\n";
    responehead += Timer::Nowtime_str() + "\r\n";
    responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
    return responehead;
}

void Badrespone::Respone200(std::string *str) {
    str->clear();
    *str += "HTTP/1.1 200 Bad Request\r\n";
    *str += "Constent_Charset:utf-8\r\n";
    *str += "Content-Language:zh-CN\r\n";
    *str += "Content-Length:0\r\n";
    *str += Timer::Nowtime_str() + "\r\n";
    *str += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}