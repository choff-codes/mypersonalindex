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

    assetAllocation(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = "(Blank)"):
        objectKey(objectType_AA, description_, id_, parent_),
        target(UNASSIGNED)
    {}

    bool operator==(const assetAllocation &other_) const;
    bool operator!=(const assetAllocation &other_) const { return !(*this == other_); }

    QString validate();

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
};

#endif // ASSETALLOCATION_H
