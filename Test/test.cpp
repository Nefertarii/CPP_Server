#include <iostream>
#include <thread>
#include "../Headfile/Thread/vector.hpp"
#include "../Headfile/Thread/list.hpp"
#include "../Headfile/Thread/queue.hpp"
#include "../Headfile/Thread/deque.hpp"
#include "../Headfile/Thread/stack.hpp"
#include "../Headfile/Thread/map.hpp"
#include "../Headfile/Thread/threadpool.hpp"
#include "../Headfile/Timer/Head/timer.h"
#include "../Log/Head/formatter.h"
#include "../Log/Head/sink.h"
#include "../Log/Head/core.h"
#include "../Headfile/Net/Head/netlog.h"
#include "../Headfile/Net/Head/eventloop.h"


/* using/enum  AaBbCc
 * class/struct Aa_Bb_Cc
 * func Aa_bb_cc
 * var aa_bb_cc
 *
 */

//using namespace Wasi::Log;
using namespace Wasi::Net;
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

/*Event_Loop eventloop;

void func2() {
    std::cout << "func2 run.\n";
    eventloop.Loop();
}

void func1() {
    std::cout << "func1 run.\n";
    eventloop.Loop();
}
//std::cout << "|" << sink.Log_consume() << "|";
*/

int main() {
    //thread T1(func1);
    //thread T2(func2);
}