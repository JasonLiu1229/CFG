#ifndef CFG_CFG_H
#define CFG_CFG_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Components;

using namespace std;

class CFG {
    //==== Main Components ====//
    vector<Components*> starter;            // Start symbol
    vector<Components*> varTer;             // Variables or Terminals
    string yield;                           // end result

    //==== Secondary Components ====//

public:
    //==== Constructors ====//
    CFG();

    //==== Functions ====//
    void print();

    //==== Destructor ====//
    virtual ~CFG();
};


#endif //CFG_CFG_H
