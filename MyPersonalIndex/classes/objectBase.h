#ifndef OBJECTBASE_H
#define OBJECTBASE_H

class objectBase
{
public:
    int parent;

    objectBase(const int &parent_ = -1): parent(parent_) {}
    bool hasParent() { return parent == -1; }
};

#endif // OBJECTBASE_H
