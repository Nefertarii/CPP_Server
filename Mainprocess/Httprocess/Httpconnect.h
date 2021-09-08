#ifndef HTTPCONNECT_H_
#define HTTPCONNECT_H_

#include "Servfunc.h"

class Httpconnect {
private:
    size_t concurrent_count;
    size_t connect_nums;
    void Reset_client(struct Clientinfo *client);

public:
    Httpconnect(){}
    int Connectlisten();
    int Canconnect();
    const int Concurrent_count();
    const int Connect_nums();
    const int Single_concurrent_client();
    void Disconnect(struct Clientinfo *client);
    ~Httpconnect(){};
};

#endif