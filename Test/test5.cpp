#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "../Net/Base/Head/buffer.h"
#include "../Net/Sockets/Head/socketapi.h"

using namespace std;
using namespace Wasi::Base;

void func1() {
    Buffer buf, buf2;
    buf.Append("123456");
    cout << buf.Content() << "\n";
    buf2 += buf;
    cout << buf2.Content() << "\n";
    buf = buf + buf;
    cout << buf.Content() << "\n";
    cout << buf.Find("34") << "\n";
}

void func2() {
    int sockfd = 0;
    sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9002);
    std::string str3 = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890~!@#$%^&*(){}:\">?\n";
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    std::cout << "connect:" << connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) << "\n";
    std::cout << "write:" << write(sockfd, str3.c_str(), str3.size()) << "bytes\n";
    close(sockfd);
}

int main() {
    func2();
}