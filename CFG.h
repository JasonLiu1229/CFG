#ifndef CFG_CFG_H
#define CFG_CFG_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

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

    //==== Conversion ====//
    void toCNF();

    //==== Destructor ====//
    virtual ~CFG();
};


#endif //CFG_CFG_H
