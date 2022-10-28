#include "CFG.h"
#include "Tree.h"
#include "Components.h"
#include "json.hpp"

#include <set>
#include <fstream>
#include <algorithm>
#include <iomanip>

using json = nlohmann::json;

CFG::CFG() : count(0){
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

CFG::CFG(const string& fileName) : count(0){
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
                i->setGen(true);
                insert = false;
            }
        }
        if (insert){
            map<Components*, bool> mappie;
            i->setGen(recurveGen(genSym, i, mappie));
        }
    }
}

bool CFG::recurveGen(vector<Components *> &genSym, Components* curSym, map<Components*, bool>& mappie) {;
    if (mappie[curSym]){
        return false;
    }
    for (auto i : curSym->getRule()){
        if (i.size() == 1 and i[0]->isGen()){
            genSym.push_back(curSym);
            curSym->setGen(true);
            return true;
        }
        else{
            bool gen = true;
            for (auto j : i){
                if (curSym == j or j->isGen()){
                    continue;
                }
                mappie[curSym] = true;
                if (!recurveGen(genSym, j, mappie)){
                    gen = false;
                }
            }
            if (gen){
                curSym->setGen(true);
                genSym.push_back(curSym);
                return gen;
            }
        }
    }
    return false;
}

void CFG::deleteProdGen(const vector<Components*>& gen) {
    vector<Components*> notGen;
    for (auto i : variables){
        if (!i->isGen()){
            notGen.push_back(i);
        }
    }
    for (auto i : gen){
        vector<vector<Components*>> items = i->getRule();
        for (auto k : notGen){
            for (auto j = items.begin(); j != items.end(); j++){
                if (find(j->begin() , j->end() , k) != j->end()){
                    j = items.erase(j);
                    j--;
                }
            }
        }
        i->setRule(items);
    }
    for (auto i : notGen){
        delete i;
    }
}


void CFG::findReachable(set<Components*>&reSym) {
    for (auto i : starter){
        reSym.insert(i);
        i->setReach(true);
        map<Components*, bool> mappie;
        recurveRe(reSym, i, mappie);
    }
}

void CFG::recurveRe(set<Components *> &reSym, Components *currentSym, map<Components*, bool> mappie) {
    for (const auto& i : currentSym->getRule()){
        for (auto j : i){
            if (mappie[j]){
                continue;
            }
            else {
                reSym.insert(j);
                j->setReach(true);
                mappie[currentSym] = true;
                recurveRe(reSym, j, mappie);
            }
        }
    }
}

void CFG::deleteProdReach(const set<Components*>& reach) {
    vector<Components*> notReach;
    for (auto i : variables){
        if (!i->isReach()){
            notReach.push_back(i);
        }
    }
    for (auto i : reach){
        vector<vector<Components*>> items = i->getRule();
        for (auto k : notReach){
            for (auto j = items.begin(); j != items.end(); j++){
                if (find(j->begin() , j->end() , k) != j->end()){
                    j = items.erase(j);
                    j--;
                }
            }
        }
        i->setRule(items);
    }
    for (auto i : notReach){
        delete i;
    }
}

void CFG::createBoddies(vector<Components*> &newVars) {
    for (auto i : variables){
        vector<vector<Components*>> items = i->getRule();
        for (auto& j : items){
            if (j.size() == 1){
                continue;
            }
            else {
                for (auto &k : j){
                    if (k->isTv()){
                        int size = variables.size();
                        k = searchForVariable(k);
                        if (size < variables.size()){
                            newVars.push_back(k);
                        }
                    }
                }
            }
        }
        i->setRule(items);
    }
}

Components *CFG::searchForVariable(Components* terminal) {
    for (auto i : variables){
        for (auto j : i->getRule()){
            if (j.size() == 1 and j[0] == terminal){
                return i;
            }
        }
    }
    auto* newComp = new Components('_' + terminal->getName(), false, {{terminal}});
    variables.push_back(newComp);
    varTer.push_back(newComp);
    return newComp;
}

void CFG::breakBoddies(Components* curSym) {
    static int number = {2};
    vector<vector<Components*>> items = curSym->getRules();
    for (auto& i : items){
        if (i.size() > 2){
            count++;
            vector<Components*> prod = {i[i.size()-2], i[i.size()-1]};
            vector<Components*> newProd;
            newProd.insert(newProd.begin(), i.begin(), i.end() - 2);
            auto* newComp = new Components(curSym->getName() + "_" + to_string(number), false, {prod});
            variables.push_back(newComp);
            varTer.push_back(newComp);
            newProd.push_back(newComp);
            i = newProd;
            curSym->setRules(items);
            number++;
            breakBoddies(curSym);
            number = 2;
            return;
        }
    }
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

    // eliminate unit pairs
    cout << " >> Eliminating unit pairs" << endl;

    set<pair<Components*, Components*>> unitP = findUnitPairs();
    vector<pair<Components*, Components*>> allpairs = findAllUnitPairs();

    vector<vector<Components*>> allPairsV;

    for (auto i : allpairs){
        allPairsV.push_back({i.first, i.second});
    }
    sort(allPairsV.begin(), allPairsV.end(), comp);

    sort(allpairs.begin(), allpairs.end());

    cout << "  Found " << unitP.size() << " unit productions" << endl;
    cout << "  Unit pairs: {";
    for (auto i : allPairsV){
        cout << '(' << i[0]->getName() << ", " << i[1]->getName() << ')';
        if (i != *allPairsV.rbegin()){
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
    cout << "  Created " << newSize << " new productions, original had " << orgSize << endl << endl;

    print();

    cout << endl;

    // eliminate useless symbols
    int orgSizeVars = variables.size();
    int orgSizeTerms = terminals.size();
    int orgSizeProds = 0;
    for (auto i : variables){
        orgSizeProds += i->getRule().size();
    }

    //* search for generating variables *//
    vector<Components*> genSymbol;
    findGen(genSymbol);
    genSymbol.erase(unique(genSymbol.begin(),genSymbol.end()), genSymbol.end());
    sort(genSymbol.begin(), genSymbol.end(), Components::comp);
    deleteProdGen(genSymbol);
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
    set<Components*> reSymbol;
    findReachable(reSymbol);
    vector<Components*> reSymbolV;
    for (auto i : reSymbol){
        reSymbolV.push_back(i);
    }
    sort(reSymbolV.begin(), reSymbolV.end(), Components::comp);
    cout << "  Reachable symbols: {";
    for (auto i : reSymbolV) {
        cout << i->getName();
        if (i != *reSymbolV.rbegin()){
            cout << ", ";
        }
        else {
            cout << "}" << endl;
        }
    }
    deleteProdReach(reSymbol);

    //* search for useful symbols *//
    cout << "  Useful symbols: {";
    for (auto i : reSymbolV) {
        cout << i->getName();
        if (i != *reSymbolV.rbegin()){
            cout << ", ";
        }
        else {
            cout << "}" << endl;
        }
    }

    vector<Components*> reVSym;
    for (auto i : reSymbol){
        reVSym.push_back(i);
    }
    varTer = reVSym;
    reset();

    cout << "  Removed " << (orgSizeVars - variables.size()) << " variables, " << (orgSizeTerms - terminals.size()) << " terminals and ";
    int newProdSize = 0;
    for (auto i : variables){
        newProdSize += i->getRule().size();
    }
    cout << (orgSizeProds - newProdSize) << " productions" << endl << endl;

    print();

    cout << endl;

    // replacing terminals wit bad bodies
    cout << " >> Replacing terminals in bad bodies" << endl;
    orgSize = variables.size();
    orgSizeProds = 0;
    for (auto i : variables){
        orgSizeProds += i->getRule().size();
    }
    vector<Components*> newVars;
    createBoddies(newVars);
    newSize = variables.size();
    newProdSize = 0;
    for (auto i : variables){
        newProdSize += i->getRule().size();
    }
    cout << "  Added " << newSize - orgSize << " new variables: {";
    sort(newVars.begin(), newVars.end(), Components::comp);
    for (auto i : newVars){
        cout << i->getName();
        if (i != *newVars.rbegin()){
            cout << ", ";
        }
    }
    cout << "}" << endl;
    cout << "  Created " << newProdSize << " new productions, original had " << orgSizeProds << endl << endl;
    print();

    cout << endl;

    // breaking apart bodies, adding new variables
    orgSizeVars = variables.size();
    for (auto i : variables){
        breakBoddies(i);
    }
    newSize = variables.size();
    vector<Components*> symbols = variables;
    sort(symbols.begin(), symbols.end(), Components::comp);
    variables = symbols;
    cout << " >> Broke " << count << " bodies, added " << newSize - orgSizeVars << " new variables" << endl;
    cout << ">>> Result CFG:" << endl << endl;
    print();
}

CFG::~CFG() {
    for (auto item : varTer) {
        delete item;
    }
}
