#include "Important/Gthread.h"
#include <iostream>
#include <list>
#include <algorithm>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(value);
}

bool list_contains(int value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value) != some_list.end();
}

void thread_add_value()
{
    add_to_list(4);
    add_to_list(5);
}

int main()
{

    add_to_list(1);
    add_to_list(2);
    add_to_list(3);
    for (int i = 0; i != 10; i++)
    {
        std::thread mythread(thread_add_value);
        mythread.join();
    }
    add_to_list(6);
    for(auto i:some_list)
        std::cout << i << " ";
    std::cout << "\n";
}