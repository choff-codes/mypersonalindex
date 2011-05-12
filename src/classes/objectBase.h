#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#ifndef UNASSIGNED
#define UNASSIGNED -1
#endif

class objectBase
{
public:
    int parent;

    objectBase(int parent_ = UNASSIGNED):
        parent(parent_)
    {}

    virtual ~objectBase() {}

    bool operator==(const objectBase &other_) const { return this->parent == other_.parent; }
    bool operator!=(const objectBase &other_) const { return !(*this == other_); }
};

#endif // OBJECTBASE_H
