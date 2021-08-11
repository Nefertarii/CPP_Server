//#include "Important/Gtime.h"

#include "Important/Gtime.h"
#include <iostream>

using namespace std;

int main()
{
    Timer timer;
    cout << timer.Nowtime_ms() << "\n";
    cout << timer.Nowtime_sec() << "\n";
    cout << timer.Nowtime_str() << "\n";
    sleep(2);
    cout << timer.Runtime_ms() << "\n";
    cout << timer.Runtime_sec() << "\n";
    cout << timer.Runtime_str() << "\n";
    return 0;
}