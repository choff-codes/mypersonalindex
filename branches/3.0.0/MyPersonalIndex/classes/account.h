#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore>
#include "queries.h"
#include "functions.h"

class account
{
public:
    enum costBasisType { costBasisType_None, costBasisType_FIFO, costBasisType_LIFO, costBasisType_AVG };

    int id;
    QString description;
    double taxRate;
    bool taxDeferred;
    costBasisType costBasis;

    account(const QString &p_description = "(Blank)"): id(-1), description(p_description), taxRate(-1), taxDeferred(false), costBasis(costBasisType_None) {}

    bool operator==(const account &other) const;
    bool operator!=(const account &other) const { return !(*this == other); }

    void save(const int &portfolioID);
    void remove() const;
};

#endif // ACCOUNT_H
