#include "GQLparser.h"
#include <iostream>

int main() {
    GQLparser gqlctrl;
    gqlctrl.GQLparse("SEARCH graph1 WHERE data1 EQUAL datavalue");
}
