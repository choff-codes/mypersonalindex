#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include <QtGui>

class assetAllocation
{
public:
    int id;
    QString description;
    double target;

    assetAllocation(): id(-1), target(-1) {}

    bool operator==(const assetAllocation &other) const
    {
        return this->id == other.id
                && this->description == other.description
                && this->target == other.target;
    }

    bool operator!=(const assetAllocation &other) const
    {
        return !(*this == other);
    }
};

#endif // ASSETALLOCATION_H
