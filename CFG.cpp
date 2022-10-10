#include "CFG.h"
#include "Components.h"
#include "json.hpp"

#include <set>
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

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

CFG::CFG(const string& fileName) {
    // inlezen uit file
    ifstream input(fileName);

    json j;
    input >> j;

    for (const auto& i : j["Variables"]){
        auto* com = new Components(i, false);
        accVarTer[i] = com;
        varTer.push_back(com);
        variables.push_back(com);
    }

    for (const auto& i : j["Terminals"]){
        auto* com = new Components(i);
        accVarTer[i] = com;
        varTer.push_back(com);
        terminals.push_back(com);
    }

    for (auto item : j["Productions"]){
        vector<Components*> productionRule;
        for (const auto& i : item["body"]){
            productionRule.push_back(accVarTer[i]);
        }
        accVarTer[item["head"]]->addRule(productionRule);
    }
    vector<Components*> start = {accVarTer[j["start"]]};
}

void CFG::print() {
    // sort variablesS and terminalsS
    set<string> variablesS;
    set<string> terminalsS;

    for (auto item : varTer) {
        if (item->isTv()){
            terminalsS.insert(item->getName());
        }
        else {
            variablesS.insert(item->getName());
        }
    }

    // print
    cout << "V = {";

    if (!variablesS.empty()) {
        auto item2 = variablesS.end();
        item2--;

        for (const auto& item: variablesS) {
            cout << item;
            if (item != *item2) {
                cout << ", ";
            }
        }
        cout << "}";
    }
    else {
        cout << "}";
    }

    cout << endl;

    cout << "T = {";

    if (!variablesS.empty()) {
        auto item2 = terminalsS.end();
        item2--;

        for (const auto& item: terminalsS) {
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

    // productions
    cout << "P = {";

    if (variablesS.empty()){
        cout << "}";
    }
    else {
        cout << endl;

        vector<Components *> variablesC;

        for (const auto &item: variablesS) {
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

    cout << endl;

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
