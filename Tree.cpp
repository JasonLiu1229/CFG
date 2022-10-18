//

#include "Tree.h"
#include "Components.h"
#include <utility>
#include <algorithm>

Tree::Tree(){}

Tree::Tree(std::vector<Components *> root) : root(std::move(root)){
    findLeaves();
}

Tree::Tree(const vector<Components *> &root, const vector<Components *> &nullSymbols,
           const vector<Components *> &epsilonSymbols) : root(root), nullSymbols(nullSymbols),
                                                         epsilonSymbols(epsilonSymbols) {
    findLeaves();

    vector<vector<Components*>> newLeaves;
    for (const auto& i : leaves){
        vector<Components*> newVec;
        for (auto j : i){
            if (j != nullptr){
                newVec.push_back(j);
            }
        }
        newLeaves.push_back(newVec);
    }
    leaves = newLeaves;
}

const std::vector<Components *> &Tree::getNullSymbols() const {
    return nullSymbols;
}

void Tree::setNullSymbols(const std::vector<Components *> &nullSymbols) {
    Tree::nullSymbols = nullSymbols;
}

const std::vector<Components *> &Tree::getEpsilonSymbols() const {
    return epsilonSymbols;
}

void Tree::setEpsilonSymbols(const std::vector<Components *> &epsilonSymbols) {
    Tree::epsilonSymbols = epsilonSymbols;
}

const vector<std::vector<Components *>> &Tree::getLeaves() const {
    return leaves;
}


void Tree::setLeaves(const vector<std::vector<Components *>> &leaves) {
    Tree::leaves = leaves;
}

void Tree::findLeaves() {
    int height = 0;
    leaves.push_back(root);
    while (height < root.size()){
        vector<vector<Components*>> tempLeaves;
        for (auto item : leaves){
            vector<Components*> newLeave1 = item;
            if (!item.empty()) {
                if (isNull(newLeave1[height])) {
                    if (newLeave1[height]->getRule().size() > 1) {
                        newLeave1[height] = {};
                        tempLeaves.push_back(newLeave1);
                        tempLeaves.push_back(item);
                    } else {
                        newLeave1[height] = {};
                        tempLeaves.push_back(newLeave1);
                    }
                } else {
                    tempLeaves.push_back(item);
                }
            }
        }
        leaves = tempLeaves;
        height++;
    }
}

bool Tree::isNull(Components *comp) {
    if (std::count(nullSymbols.begin(), nullSymbols.end(), comp)){
        return true;
    }
    else {
        return false;
    }
}

Tree::~Tree() {

}
