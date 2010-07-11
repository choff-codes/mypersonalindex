#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include <QString>
#include "queries.h"
#include "functions.h"
#include "objectKey.h"

class assetAllocation: public objectKey
{
public:
    double target;

    assetAllocation(int id_ = -1, int parent_ = -1, const QString &description_ = "(Blank)"):
        objectKey(objectType_AA, description_, id_, parent_),
        target(-1)
    {}

    bool operator==(const assetAllocation &other_) const;
    bool operator!=(const assetAllocation &other_) const { return !(*this == other_); }

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
};

#endif // ASSETALLOCATION_H
