#include "CFG.h"
#include "Tree.h"
#include "Components.h"
#include "json.hpp"

#include <set>
#include <fstream>
#include <algorithm>
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
        accVarTer[item["head"]]->addRuleSort(productionRule);
    }
    vector<Components*> start = {accVarTer[j["Start"]]};
    starter = start;
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

/*bool CFG::epsilonExist() {      // true: if there is an epsilon production
    for (auto i : variables) {
        for (auto j : i->getRule()){
            if (j.empty()){
                return true;
            }
        }
    }
    return false;
}*/

vector<Components *> CFG::epsilonVariables() {
    vector<Components*> epsilonVariables;

    for (auto i : variables) {
        for (const auto &j: i->getRule()) {
            if (j.empty()) {
                epsilonVariables.push_back(i);
                break;
            }
        }

    }
    return epsilonVariables;
}

vector<Components *> CFG::nullVariables() {
    vector<Components*> nullVars;
    for (auto item1 : variables){
        if (isNull(item1)){
            nullVars.push_back(item1);
        }
    }
    return nullVars;
}

bool CFG::findIntersection(const vector<Components*>& set1, const vector<Components*>& set2) {
    vector<Components*> set3;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(set3));
    return !set3.empty();
}

bool CFG::isNull(Components* comp) {
    for (const auto& item1 : comp->getRule()){
        if (item1.empty()){
            return true;
        }
        else {
            for (auto item2 : item1){
                return isNull(item2);
            }
        }
    }
    return false;
}

set<pair<Components *, Components *>> CFG::findUnitPairs() {
    set<pair<Components *, Components *>> setPairs;
    for (auto i : variables){
        for (auto j : i->getRule()){
            if (j.size() == 1 and !j[0]->isTv()){
                setPairs.insert({i, j[0]});
            }
        }
    }
    return setPairs;
}

void CFG::toCNF() {         // converts cfg to cnf (Chompsky normal form)
    // original
    cout << "Original CFG:" << endl << endl;

    print();

    cout << endl;

    // eliminate epsilon
    vector<Components*> nullVars = nullVariables();
    vector<Components*> epsVars = epsilonVariables();

    for (auto i : variables){
        vector<vector<Components*>> allProds;
        for (auto  j : i->getRule()){
            vector<vector<Components*>> prods;
            if (!j.empty()){
                Tree tree(j, nullVars, epsVars);
                prods = tree.getLeaves();
            }
            if (findIntersection({i}, epsVars)){
                allProds.insert(allProds.end(), prods.begin(), prods.end());
                allProds.erase(unique(allProds.begin(), allProds.end()), allProds.end());
            }
        }
        if (!allProds.empty()){
            i->setRule(allProds);
            i->cleanUp();
        }
    }

    print();

    // eliminate unit pairs
    set<pair<Components*, Components*>> unitP = findUnitPairs();

    for (auto i : unitP){
        Components* firstVar = i.first;
        Components* secondVar = i.second;
        firstVar->deleteProduction({secondVar});
        firstVar->addRules(secondVar->getRule());
        firstVar->cleanUp();
    }

    for (auto i : variables){
        if (i->getRule().empty()){
            for (auto j : variables){
                j->deleteProduction({i});
            }
        }
    }

    print();

    // eliminate useless symbols
    for (auto i : variables){
        if (i->getRule().empty()){
            // search for
        }
    }

    // replacing terminals wit bad bodies


    // breaking apart bodies, adding new variables


}

CFG::~CFG() {
    for (auto item : varTer) {
        delete item;
    }
}
