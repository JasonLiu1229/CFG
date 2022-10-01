//
// Created by liuja on 01/10/2022.
//

#include "CFG.h"
#include <set>

CFG::CFG() {
    // Variables
    auto* S = new Components("S", false);
    auto* BINDIGIT = new Components("BINDIGIT", false);

    varTer.push_back(S);
    varTer.push_back(BINDIGIT);

    // Terminals
    auto* a = new Components("a");
    auto* zero = new Components("0");
    auto* b = new Components("b");
    auto* one = new Components("1");

    varTer.push_back(one);
    varTer.push_back(a);
    varTer.push_back(zero);
    varTer.push_back(b);

    // Productions
    vector<Components*> prod;

    prod = {zero};
    BINDIGIT->addRule(prod);

    prod = {one};
    BINDIGIT->addRule(prod);

    prod = {};
    S->addRule(prod);

    prod = {a, S, b, BINDIGIT};
    S->addRule(prod);

    // Starter
    starter = {S};
}

void CFG::print() {
    // sort variables and terminals
    set<string> variables;
    set<string> terminals;

    for (auto item : varTer) {
        if (item->isTv()){
            terminals.insert(item->getName());
        }
        else {
            variables.insert(item->getName());
        }
    }

    // print
    cout << "V = {";

    if (!variables.empty()) {
        auto item2 = variables.end();
        item2--;

        for (const auto& item: variables) {
            cout << item;
            if (item != *item2) {
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }
    else {
        cout << "}" << endl;
    }

    cout << endl;

    cout << "T = {";

    if (!variables.empty()) {
        auto item2 = terminals.end();
        item2--;

        for (const auto& item: terminals) {
            cout << item;
            if (item != *item2) {
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }
    else {
        cout << "}" << endl;
    }

    cout << endl;

    // productions
    cout << "P = {";

    if (variables.empty()){
        cout << "}";
    }
    else {
        cout << endl << endl;

        vector<Components *> variablesC;

        for (const auto &item: variables) {
            for (auto item2: varTer) {
                if (item2->getName() == item) {
                    variablesC.push_back(item2);
                    break;
                }
            }
        }

        for (auto item : variablesC) {
            item->printProd();
        }
        cout << "}";
    }

    cout << endl << endl;

    // starter

    cout << "S = ";

    for (auto item : starter){
        cout << item->getName() << " ";
    }
    cout << endl;
}

CFG::~CFG() {
    for (auto item : varTer) {
        delete item;
    }
}
