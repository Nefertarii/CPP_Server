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


/* using/enum  AaBbCc
 * class/struct Aa_Bb_Cc
 * func Aa_bb_cc
 * var aa_bb_cc
 *
 */

Thread::Safe_Queue<int> ique;
Thread::Safe_Deque<int> ideq;
Thread::Safe_Vector<int> ivec;
Thread::Safe_Stack<int> istk;
Thread::Safe_List<int> ilist;
Thread::Safe_Map<int, int> iimap;
Thread::Thread_Pool threads;

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
    Core core1;
    core1.Normal_log("Test1", "detail111111");
    core1.Normal_log("Test2", "detail111111");
    core1.Normal_log("Test3", "detail111111");
    core1.Normal_log("Test4", "detail111111");
    core1.Normal_log("Test5", "detail111111");
    core1.Save_to_file();
}
//std::cout << "|" << sink.Log_consume() << "|";


int main() {
    func1();
    std::cout << "hi123\n";
}