#include "Httperrno.h"
#include <iostream>
using namespace std;

int main() {
    cout << StrCode::StrHttpCode((HTTPSTATE)202);
}