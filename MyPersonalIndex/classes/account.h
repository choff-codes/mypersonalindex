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
    costBasis costBasis;

    account(const int &id_ = -1, const int &parent_ = -1, const QString &description_ = "(Blank)"):
        objectKey(objectType_Account, description_, id_, parent_),
        taxRate(-1),
        taxDeferred(false),
        costBasis(costBasis_None)
    {}

    bool operator==(const account &other) const;
    bool operator!=(const account &other) const { return !(*this == other); }

    void save(const queries &dataSource);
    void remove(const queries &dataSource) const;
};

#endif // ACCOUNT_H
