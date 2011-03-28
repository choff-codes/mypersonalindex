#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "objectKey.h"

class QString;
class QSqlQuery;
class queries;
class accountData;
class account: public objectKey
{
public:
    enum costBasisMethod {
        costBasisMethod_FIFO,
        costBasisMethod_LIFO,
        costBasisMethod_AVG,
        costBasisMethod_HIFO
    };

    account();
    account(const account &other_);

    ~account();

    account& operator=(const account &other_);

    bool operator==(const account &other_) const;
    bool operator!=(const account &other_) const { return !(*this == other_); }

    double taxRate() const;
    void setTaxRate(double taxRate_);

    bool taxDeferred() const;
    void setTaxDeferred(bool taxDeferred_);

    costBasisMethod costBasis() const;
    void setCostBasis(costBasisMethod costBasis_);

    bool hidden() const;
    void setHidden(bool hidden_);

    objectType type() const;

    QString validate() const;

    bool save(const queries &dataSource_) const;
    static account load(const QSqlQuery &q_);

    friend QDataStream& operator>>(QDataStream &stream_, account &acct_);

    void detach();

private:
    QExplicitlySharedDataPointer<accountData> d;

    objectKeyData* data() const;
};

QDataStream& operator<<(QDataStream &stream_, const account &acct_);
QDataStream& operator>>(QDataStream &stream_, account &acct_);

#endif // ACCOUNT_H
