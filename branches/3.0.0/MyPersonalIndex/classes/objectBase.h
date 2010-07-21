#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#ifndef UNASSIGNED
#define UNASSIGNED -1
#endif

class objectBase
{
public:
    int parent;

    objectBase(int parent_ = -1):
        parent(parent_)
    {}

    bool hasParent() const { return parent == UNASSIGNED; }
};

#endif // OBJECTBASE_H
