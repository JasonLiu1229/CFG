//
// Created by liuja on 01/10/2022.
//

#include "Components.h"

#include <utility>

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
    rules.push_back(rule);
}

Components::~Components() {

}
