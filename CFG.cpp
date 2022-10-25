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

void CFG::allUnitPairs(vector<pair<Components *, Components *>> &allPairs, Components *currentSymbol, Components *nextSymbol) {
    vector<vector<Components*>> nextProductions;
    nextProductions = nextSymbol == nullptr ? currentSymbol->getRule() : nextSymbol->getRule();
    for (auto i : nextProductions){
        if (i.size() == 1 and !i[0]->isTv()){
            allPairs.emplace_back(currentSymbol, i[0]);
            allUnitPairs(allPairs, currentSymbol, i[0]);
        }
    }
}

vector<pair<Components *, Components *>> CFG::findAllUnitPairs() {
    vector<pair<Components *, Components *>> allPairs;

    for (auto i : variables){
        allPairs.emplace_back(i,i);
        allUnitPairs(allPairs, i);
    }

    set<pair<Components *, Components *>> setPair;
    for (auto i : allPairs){
        setPair.insert(i);
    }
    allPairs = {};
    for (auto i : setPair){
        allPairs.push_back(i);
    }
    return allPairs;
}

int CFG::sizeOfProds() {
    int size = 0;
    for (auto i : variables){
        size += i->getRule().size();
    }
    return size;
}


void CFG::findGen(vector<Components *> &genSym) {
    for (auto i : terminals){
        i->setGen(true);
        genSym.push_back(i);
    }
    for (auto i : variables){
        bool insert = true;
        for (auto j : i->getRule()) {
            bool terminal = true;
            for (auto k : j){
                if (!k->isGen()){
                    terminal = false;
                }
            }
            if (terminal){
                genSym.push_back(i);
                insert = false;
            }
        }
        if (insert){
            map<Components*, bool> mappie;
            i->setGen(recurveGen(genSym, i, mappie));
        }
    }
    /*vector<Components*> res;
    set_difference(varTer.begin(), varTer.end(), genSym.begin(), genSym.end(), back_inserter(res, res.begin()));
    for (auto i : res){
        map<Components*, bool> mappie;
        recurveGen(genSym, i, mappie);
    }*/
}

bool CFG::recurveGen(vector<Components *> &genSym, Components* curSym, map<Components*, bool>& mappie) {;
    if (mappie[curSym]){
        return false;
    }
    for (auto i : curSym->getRule()){
        if (i.size() == 1 and i[0]->isGen()){
            genSym.push_back(curSym);
            return true;
        }
        else{
            bool gen = true;
            for (auto j : i){
                if (curSym == j){
                    continue;
                }
                mappie[curSym] = true;
                if (!recurveGen(genSym, j, mappie)){
                    gen = false;
                }
            }
            if (gen){
                return gen;
            }
        }
    }
    return false;
}

void CFG::findReachable(vector<Components*>&reSym) {

}


void CFG::recurveRe(vector<Components *> &reSym, Components *currentSym, map<Components*, bool> mappie) {

}

bool CFG::comp(vector<Components *> x, vector<Components *> y) {
    for (int i = 0; i < min(x.size(), y.size()); ++i) {
        if (x[i] != y[i]){
            return Components::comp(x[i], y[i]);
        }
        else if (x[i] == y[i] and i == min(x.size(), y.size()) - 1){
            return min(x.size(), y.size()) == y.size();
        }
    }
    return false;
}

void CFG::reset() {
    vector<Components*> vars;
    vector<Components*> terms;
    for (const auto i : varTer){
        if (i->isTv()){
            terms.push_back(i);
        }
        else {
            vars.push_back(i);
        }
    }
    variables = vars;
    terminals = terms;
}

void CFG::toCNF() {         // converts cfg to cnf (Chompsky normal form)
    // original
    cout << "Original CFG:" << endl << endl;

    print();

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;

    // eliminate epsilon
    int orgSize = sizeOfProds();
    cout << " >> Eliminating epsilon productions" << endl;
    vector<Components*> nullVars = nullVariables();
    vector<Components*> epsVars = epsilonVariables();
    cout << "  Nullables are {";
    sort(nullVars.begin(), nullVars.end());
    for (auto i : nullVars){
        cout << i->getName();
        if (i != *nullVars.rbegin()){
            cout << ", ";
        }
    }
    cout << "}" << endl;

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
            sort(allProds.begin(),allProds.end(), comp);
            i->setRule(allProds);
        }
    }

    int newSize = sizeOfProds();
    cout << "  Created " << newSize << " productions, original had " << orgSize << endl << endl;

    print();

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;

    // eliminate unit pairs
    cout << " >> Eliminating unit pairs" << endl;

    set<pair<Components*, Components*>> unitP = findUnitPairs();
    vector<pair<Components*, Components*>> allpairs = findAllUnitPairs();

    sort(allpairs.begin(), allpairs.end());

    cout << "  Found " << unitP.size() << " unit productions" << endl;
    cout << "  Unit pairs: {";
    for (auto i : allpairs){
        cout << '(' << i.first->getName() << ", " << i.second->getName() << ')';
        if (i != *allpairs.rbegin()){
            cout << ", ";
        }
    }
    cout << '}' << endl;

    orgSize = newSize;

    for (auto i : unitP){
        Components* firstVar = i.first;
        Components* secondVar = i.second;
        firstVar->deleteProduction({secondVar});
        firstVar->addRules(secondVar->getRule());
        vector<vector<Components*>> allProds = firstVar->getRule();
        sort(allProds.begin(),allProds.end(), comp);
        firstVar->setRule(allProds);
    }

    for (auto i : variables){
        if (i->getRule().empty()){
            for (auto j : variables){
                j->deleteProduction({i});
            }
        }
    }

    newSize = sizeOfProds();
    cout << "  Created " << newSize << " productions, original had " << orgSize << endl << endl;

    print();

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;

    // eliminate useless symbols
    int orgSizeVars = variables.size();
    int orgSizeTerms = terminals.size();

    //* search for generating variables *//
    vector<Components*> genSymbol;
    findGen(genSymbol);
    genSymbol.erase(unique(genSymbol.begin(),genSymbol.end()), genSymbol.end());
    sort(genSymbol.begin(), genSymbol.end(), Components::comp);
    varTer = genSymbol;
    reset();
    cout << " >> Eliminating useless symbols" << endl;
    cout << "  Generating symbols: {";
    for (auto i : genSymbol) {
        cout << i->getName();
        if (i != *genSymbol.rbegin()){
            cout << ", ";
        }
        else {
            cout << "}" << endl;
        }
    }

    //* search for reachable symbols *//
    vector<Components*> reSymbol;


    //* search for useful symbols *//

    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;

    // replacing terminals wit bad bodies


    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;

    // breaking apart bodies, adding new variables


    cout << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;
}

CFG::~CFG() {
    for (auto item : varTer) {
        delete item;
    }
}
