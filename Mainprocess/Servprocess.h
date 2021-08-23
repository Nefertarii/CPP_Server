#ifndef SERVPROCESS_H_
#define SERVPROCESS_H_

class Connectaccept {
public:
    Connectaccept(){};
    virtual void Connectlisten() = 0;
    virtual int Client_accept() = 0;
    virtual ~Connectaccept() = 0;
};

class Connectprocess {
public:
    Connectprocess(){};
    virtual void Send() = 0;
    virtual void Sendfile() = 0;
    virtual void Read() = 0;
    virtual void Disconnect() = 0;
    virtual ~Connectprocess() = 0;
};

#endif