#ifndef CFG_TREE_H
#define CFG_TREE_H
#include <vector>

class Components;

class Tree {
    std::vector<Components*> root;
    std::vector<std::vector<Components*>> leaves;
    std::vector<Components*> nullSymbols;
    std::vector<Components*> epsilonSymbols;

public:
    //==== Constructors ====//
    Tree();

    explicit Tree(std::vector<Components *> root);

    Tree(const std::vector<Components *> &root, const std::vector<Components *> &nullSymbols,
         const std::vector<Components *> &epsilonSymbols);

    //==== getters and setters ====//
    const std::vector<Components *> &getNullSymbols() const;

    void setNullSymbols(const std::vector<Components *> &nullSymbols);

    const std::vector<Components *> &getEpsilonSymbols() const;

    void setEpsilonSymbols(const std::vector<Components *> &epsilonSymbols);

    const std::vector<std::vector<Components *>> &getLeaves() const;

    void setLeaves(const std::vector<std::vector<Components *>> &leaves);

    //==== Destructors ====//
    virtual ~Tree();

private:
    //==== Functions ====//
    void findLeaves();

    bool isNull(Components* comp);
};


#endif //CFG_TREE_H
