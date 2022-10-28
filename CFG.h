#ifndef CFG_CFG_H
#define CFG_CFG_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <set>

class Components;

using namespace std;

class CFG {
    //==== Main Components ====//
    vector<Components*> starter;            // Start symbol
    vector<Components*> varTer;             // Variables or Terminals
    vector<Components*> variables;          // Variables
    vector<Components*> terminals;          // Terminals
    map<string , Components*> accVarTer;    // accessible var and ter
    string yield;                           // end result

    //==== Secondary Components ====//
    int count;
public:
    //==== Constructors ====//
    CFG();

    CFG(const string& fileName);

    //==== Functions ====//
    void print();

    //bool epsilonExist();

    vector<Components*> epsilonVariables();

    vector<Components*> nullVariables();

    bool findIntersection(const vector<Components*>& set1, const vector<Components*>& set2);

    bool isNull(Components* comp);

    set<pair<Components*, Components*>> findUnitPairs();

    void allUnitPairs(vector<pair<Components*, Components*>> &allPairs,Components* currentSymbol, Components* nextSymbol = nullptr);

    vector<pair<Components*, Components*>> findAllUnitPairs();

    int sizeOfProds();

    void findGen(vector<Components*>& genSym);

    bool recurveGen(vector<Components*>& genSym, Components* curSym, map<Components*, bool>& mappie);

    void deleteProdGen(const vector<Components*>& gen);

    void findReachable(set<Components*>& reSym);

    void recurveRe(set<Components*>& reSym, Components* currentSym, map<Components*, bool> mappie);

    void deleteProdReach(const set<Components*>& reach);

    void createBoddies(vector<Components*> &newVars);

    void breakBoddies(Components* curSym);

    Components* searchForVariable(Components* terminal);

    static bool comp(vector<Components*> x, vector<Components*> y);

    void reset();

    //==== Conversion ====//
    void toCNF();

    //==== Destructor ====//
    virtual ~CFG();
};


#endif //CFG_CFG_H
