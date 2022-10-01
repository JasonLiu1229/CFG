//
// Created by liuja on 01/10/2022.
//

#ifndef CFG_COMPONENTS_H
#define CFG_COMPONENTS_H

#include <string>
#include <vector>

using namespace std;

class Components {
    //==== Main Variables ====//
    string name;                        // Component name
    bool TV;                            // true = terminal and false = variable
    vector<vector<Components*>> rules;   // production rule if TV = false

public:
    //==== Constructors ====//
    Components();

    Components(string name, bool tv = true);

    Components(string name, bool tv, const vector<vector<Components *>> &rules);

    //==== Getters and Setters ====//
    const string &getName() const;

    void setName(const string &name);

    bool isTv() const;

    void setTv(bool tv);

    const vector<vector<Components *>> &getRule() const;

    void setRule(const vector<vector<Components *>> &rule);

    //==== Functions ====//
    void addRule(const vector<Components*> &rule);

    void printProd();

    //==== Destructors ====//
    virtual ~Components();
};


#endif //CFG_COMPONENTS_H
