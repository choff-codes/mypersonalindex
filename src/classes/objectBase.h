#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#ifndef UNASSIGNED
#define UNASSIGNED -1
#endif

// any value between this may be used for other purposes
// use UNASSIGNED_RESERVED - 1 for new objects

#ifndef UNASSIGNED_RESERVED
#define UNASSIGNED_RESERVED -10
#endif

class objectBase
{
public:
    int parent;

    objectBase(int parent_ = UNASSIGNED):
        parent(parent_)
    {}

    bool operator==(const objectBase &other_) const { return this->parent == other_.parent; }
    bool operator!=(const objectBase &other_) const { return !(*this == other_); }

    bool hasParent() const { return parent == UNASSIGNED; }
};

#endif // OBJECTBASE_H
