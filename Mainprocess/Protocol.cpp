#include "Protocol.h"

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

