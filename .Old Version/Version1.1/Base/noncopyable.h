#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

class Noncopyable {
protected:
    Noncopyable()  = default;
    ~Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    const Noncopyable& operator=(const Noncopyable&) = delete;
};

#endif