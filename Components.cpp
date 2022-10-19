#include "Components.h"

#include <utility>
#include <iostream>
#include <algorithm>
#include <set>

Components::Components() {}

Components::Components(string name, bool tv) : name(std::move(name)), TV(tv) {}

Components::Components(string name, bool tv, const vector<vector<Components *>> &rule) : name(std::move(name)), TV(tv),
                                                                                                rules(rule) {}

const string &Components::getName() const {
    return name;
}

void Components::setName(const string &name) {
    Components::name = name;
}

bool Components::isTv() const {
    return TV;
}

void Components::setTv(bool tv) {
    TV = tv;
}

const vector<vector<Components *>> &Components::getRule() const {
    return rules;
}

void Components::setRule(const vector<vector<Components *>> &rule) {
    Components::rules = rule;
}

void Components::addRule(const vector<Components *> &rule) {
    if (rule.empty()){
        rules.insert(rules.begin(), rule);
    }
    else {
        rules.push_back(rule);
    }
}

void Components::addRules(const vector<vector<Components *>> &newRules) {
    set<vector<Components*>> setRules;
    for (const auto& i : rules){
        setRules.insert(i);
    }
    for (const auto& i : newRules){
        setRules.insert(i);
    }
    vector<vector<Components*>> vecRules;
    for (const auto& i : setRules){
        vecRules.push_back(i);
    }
    rules = vecRules;
}

void Components::cleanUp() {
    sort(rules.begin(), rules.end());
}

void Components::addRuleSort(const vector<Components *> &rule) {
    rules.push_back(rule);
    sort(rules.begin(), rules.end());
}


void Components::printProd() {
    for (const auto& item : rules){
        cout << "    " << name << " -> `";
        for (int i = 0; i < item.size(); ++i) {
            cout << item[i]->name;
            if (i != item.size()-1){
                 cout << " ";
            }
        }
        cout << "`"<< endl;
    }
}

void Components::deleteEpsilonProd() {
    for (int i = 0; i < rules.size(); ++i) {
        if(rules[i].empty()){
            rules.erase(rules.begin()+i);
            break;
        }
    }
}

void Components::deleteProduction(const vector<Components*> &delVector) {
    for (int i = 0; i < rules.size(); ++i) {
        if (rules[i] == delVector){
            rules.erase(rules.begin() + i);
            break;
        }
    }
}

bool Components::operator==(const string &cName) const{
    return cName == name;
}

bool Components::operator!=(const string &cName) const {
    return !(cName == name);
}

Components::~Components() {

}


