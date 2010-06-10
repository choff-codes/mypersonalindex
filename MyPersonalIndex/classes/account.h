#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include "queries.h"
#include "functions.h"
#include "objectKey.h"

class account: public objectKey
{
public:
    enum costBasisType { costBasisType_None, costBasisType_FIFO, costBasisType_LIFO, costBasisType_AVG, costBasisType_HIFO };

    double taxRate;
    bool taxDeferred;
    costBasisType costBasis;

    account(const QString &p_description = "(Blank)"):
        objectKey(objectType_Account, p_description), taxRate(-1), taxDeferred(false), costBasis(costBasisType_None) {}

    bool operator==(const account &other) const;
    bool operator!=(const account &other) const { return !(*this == other); }

    void save(const int &portfolioID);
    void remove() const;
};

#endif // ACCOUNT_H
