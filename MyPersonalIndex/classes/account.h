#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include "queries.h"
#include "functions.h"
#include "costBasis.h"
#include "objectKey.h"

class account: public objectKey
{
public:
    double taxRate;
    bool taxDeferred;
    costBasis overrideCostBasis;

    account(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = "(Blank)"):
        objectKey(objectType_Account, description_, id_, parent_),
        taxRate(UNASSIGNED),
        taxDeferred(false),
        overrideCostBasis(costBasis_None)
    {}

    bool operator==(const account &other_) const;
    bool operator!=(const account &other_) const { return !(*this == other_); }

    QString validate();

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
};

#endif // ACCOUNT_H
