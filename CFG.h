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

    //==== Destructor ====//
    virtual ~CFG();
};


#endif //CFG_CFG_H
