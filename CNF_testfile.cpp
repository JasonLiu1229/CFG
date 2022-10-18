#include "CFG.h"

using namespace std;

int main() {
    CFG cfg("CNF.json");
    cfg.toCNF();
    return 0;
}