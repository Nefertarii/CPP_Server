#include <iostream>
#include <thread>
#include "../Headfile/Thread/vector.hpp"
#include "../Headfile/Thread/list.hpp"
#include "../Headfile/Thread/queue.hpp"
#include "../Headfile/Thread/deque.hpp"
#include "../Headfile/Thread/stack.hpp"
#include "../Headfile/Thread/map.hpp"
#include "../Headfile/Thread/threadpool.hpp"
#include "../Headfile/Clock/Head/timer.h"
#include "../Log/Head/formatter.h"
#include "../Log/Head/sink.h"
#include "../Log/Head/core.h"
#include "../Headfile/Socket/Net/Head/netlog.h"


/* using/enum  AaBbCc
 * class/struct Aa_Bb_Cc
 * func Aa_bb_cc
 * var aa_bb_cc
 *
 */

//using namespace Wasi::Log;
//using namespace Wasi::Socket;
using namespace std;

Wasi::Thread::Safe_Queue<int> ique;
Wasi::Thread::Safe_Deque<int> ideq;
Wasi::Thread::Safe_Vector<int> ivec;
Wasi::Thread::Safe_Stack<int> istk;
Wasi::Thread::Safe_List<int> ilist;
Wasi::Thread::Safe_Map<int, int> iimap;
Wasi::Thread::Thread_Pool threads;

void test_func1() {
    std::cout << "test_func 1\n";
}

int test_func2() {
    std::cout << "test_func 2\nreturn 42\n";
    return 42;
}

void test_func3() {
    uint i = 0;
    while (i < 20) {
        std::cout << "test_func 3\n";
        i++;
    }
}

void func1() {
    //Wasi::Log::Core core;
    Wasi::Socket::Logging logging;
    for (int i = 0; i <= 500; i++) {
        logging.Critical_log("Test", "123");
    }
}
//std::cout << "|" << sink.Log_consume() << "|";

int main() {
    func1();
    std::cout << "hi\n";
}