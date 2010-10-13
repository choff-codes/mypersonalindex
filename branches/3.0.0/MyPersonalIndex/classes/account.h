#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include "queries.h"
#include "functions.h"
#include "objectKey.h"

class account: public objectKey
{
public:
    enum costBasisMethod {
        costBasisMethod_FIFO,
        costBasisMethod_LIFO,
        costBasisMethod_AVG,
        costBasisMethod_HIFO
    };

    double taxRate;
    bool taxDeferred;
    costBasisMethod costBasis;

    account(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = QString()):
        objectKey(description_, id_, parent_),
        taxRate(0),
        taxDeferred(false),
        costBasis(costBasisMethod_FIFO)
    {}

    bool operator==(const account &other_) const;
    bool operator!=(const account &other_) const { return !(*this == other_); }

    objectType type() const { return objectType_Account; }
    QString validate() const;

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static account load(const QSqlQuery &q_);
};

#endif // ACCOUNT_H
