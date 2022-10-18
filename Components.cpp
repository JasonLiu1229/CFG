//
// Created by liuja on 01/10/2022.
//

#include "Components.h"

#include <utility>
#include <iostream>
#include <algorithm>

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
    set_union(newRules.begin(), newRules.end(), rules.begin(), rules.end(), back_inserter(rules));
    /*cleanUp();*/
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

bool Components::operator==(const string &cName) const{
    return cName == name;
}

bool Components::operator!=(const string &cName) const {
    return !(cName == name);
}

Components::~Components() {

}

bool Components::inVector(vector<vector<Components*>> comps, vector<Components*> comp) {
    for (auto i : comps){
        if (i == comp){
            return true;
        }
    }
    return false;
}


