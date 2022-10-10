#include <iostream>
#include "CFG.h"

using namespace std;

int main() {
    CFG cfg("cfg.json");
    cfg.print();
    return 0;
}