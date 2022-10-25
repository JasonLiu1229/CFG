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
    string name;                                // Component name
    vector<vector<Components*>> rules;          // production rule if TV = false
    bool TV;                                    // true = terminal and false = variable
    bool gen;                                   // generating

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

    bool isGen() const;

    void setGen(bool gen);

    //==== Functions ====//
    void addRule(const vector<Components*> &rule);

    void addRules(const vector<vector<Components*>> &newRules);

    void addRuleSort(const vector<Components*> &rule);

    void cleanUp();

    void printProd();

    void sortProd();

    void deleteEpsilonProd();

    void deleteProduction(const vector<Components*> &delVector);

    static bool comp(Components* x, Components* b){return *x<*b;}

    //==== Operator overload ====//
    bool operator==(const string &cName) const;

    bool operator!=(const string &cName) const;

    bool operator<(const Components &rhs) const;

    bool operator>(const Components &rhs) const;

    bool operator<=(const Components &rhs) const;

    bool operator>=(const Components &rhs) const;


    //==== Destructors ====//
    virtual ~Components();
};


#endif //CFG_COMPONENTS_H
