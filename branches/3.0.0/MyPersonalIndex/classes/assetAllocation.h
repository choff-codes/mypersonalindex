#ifndef ASSETALLOCATION_H
#define ASSETALLOCATION_H

#include <QtCore>
#include "queries.h"
#include "functions.h"

class assetAllocation
{
public:
    int id;
    QString description;
    double target;

    assetAllocation(const QString &p_description = QString()): id(-1), description(p_description), target(-1) {}

    bool operator==(const assetAllocation &other) const;
    bool operator!=(const assetAllocation &other) const { return !(*this == other); }

    void save(const int &portfolioID);
    void remove() const;
};

#endif // ASSETALLOCATION_H
