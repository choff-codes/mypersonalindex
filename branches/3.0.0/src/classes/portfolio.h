#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "objectKey.h"

template <class T, class V>
class QMap;
class QSqlQuery;
class QStringList;
class queries;
class security;
class assetAllocation;
class account;
class portfolioData;
class portfolio: public objectKey
{
public:
    portfolio(int id_ = UNASSIGNED, const QString &description_ = QString());
    portfolio(const portfolio &other_);

    ~portfolio();

    portfolio& operator=(const portfolio &other_);

    bool operator==(const portfolio &other_) const;
    bool operator!=(const portfolio &other_) const { return !(*this == other_); }

    QStringList symbols() const;
    static QMap<QString, int> symbols(const QMap<int, portfolio> &portfolios_);
    int endDate() const;

    static int getOpenIdentity() { return --IDENTITY_COUNTER; }

    int startDate() const;
    void setStartDate(int startDate_);

    QMap<int, security>& securities() const;
    QMap<int, assetAllocation>& assetAllocations() const;
    QMap<int, account>& accounts() const;

    objectType type() const;
    QString validate() const;

    void save(const queries &dataSource_);
    static QMap<int, int> save(QMap<int, portfolio> &portfolios_, const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static portfolio load(const QSqlQuery &q_);

    void detach();

private:
    static int IDENTITY_COUNTER;
    QExplicitlySharedDataPointer<portfolioData> d;

    objectKeyData* data() const;
};

#endif // PORTFOLIO_H
