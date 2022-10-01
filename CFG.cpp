//
// Created by liuja on 01/10/2022.
//

#include "CFG.h"

CFG::CFG() {
    // Variables
    auto* BINDIGIT = new Components("BINDIGIT", false);
    auto* S = new Components("S", false);

    varTer.push_back(BINDIGIT);
    varTer.push_back(S);

    // Terminals
    auto* zero = new Components("0");
    auto* one = new Components("1");
    auto* a = new Components("a");
    auto* b = new Components("b");
    auto* epsilon = new Components("");

    varTer.push_back(zero);
    varTer.push_back(one);
    varTer.push_back(a);
    varTer.push_back(b);

    // Productions
    vector<Components*> prod;

    prod = {zero};
    BINDIGIT->addRule(prod);

    prod = {one};
    BINDIGIT->addRule(prod);

    prod = {epsilon};
    S->addRule(prod);

    prod = {a, S, b, BINDIGIT};
    S->addRule(prod);

    // Starter
    starter = {S};
}

void CFG::print() {
    // sort variables and terminals
    vector<Components*> variables;
    vector<Components*> terminals;

    for (auto item : varTer) {
        if (item->isTv()){
            terminals.push_back(item);
        }
        else {
            terminals.push_back(item);
        }
    }

    // sort alphabetically

}

CFG::~CFG() {
    for (auto item : varTer) {
        delete item;
    }
}
