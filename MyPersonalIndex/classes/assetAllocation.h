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

    assetAllocation(const QString &p_description = "(Blank)"):
        objectKey(objectType_AA, p_description), target(-1) {}

    bool operator==(const assetAllocation &other) const;
    bool operator!=(const assetAllocation &other) const { return !(*this == other); }

    void save(const int &portfolioID);
    void remove() const;
};

#endif // ASSETALLOCATION_H
