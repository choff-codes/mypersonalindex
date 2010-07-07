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

    assetAllocation(const int &id_ = -1, const int &parent_ = -1, const QString &description_ = "(Blank)"):
        objectKey(objectType_AA, description_, id_, parent_),
        target(-1)
    {}

    bool operator==(const assetAllocation &other) const;
    bool operator!=(const assetAllocation &other) const { return !(*this == other); }

    void save(const queries &dataSource);
    void remove(const queries &dataSource) const;
};

#endif // ASSETALLOCATION_H
