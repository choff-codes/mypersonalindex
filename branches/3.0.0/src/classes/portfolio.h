#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "objectKey.h"

template <class T, class V>
class QMap;
class QSqlQuery;
class queries;
class security;
class assetAllocation;
class symbol;
class account;
class snapshot;
class snapshotSecurity;
class historicalNAV;
class portfolioData;
class portfolio: public objectKey
{
public:
    portfolio();
    portfolio(const portfolio &other_);

    ~portfolio();

    portfolio& operator=(const portfolio &other_);

    bool operator==(const portfolio &other_) const;
    bool operator!=(const portfolio &other_) const { return !(*this == other_); }

    QStringList symbols() const;
    static QMap<QString, int> symbols(const QMap<int, portfolio> &portfolios_);
    int endDate() const;

    int startDate() const;
    void setStartDate(int startDate_);

    QMap<int, security>& securities() const;
    QMap<int, assetAllocation>& assetAllocations() const;
    QMap<int, account>& accounts() const;

    objectType type() const;
    QString validate() const;
    void setID(int id_);

    static bool save(const QMap<int, portfolio> &portfolios_, const queries &dataSource_);
    static portfolio load(const QSqlQuery &q_);

    void clearCache() const;

    snapshotSecurity securitySnapshot(int date_, int id_, int priorDate_ = 0) const;
    snapshot portfolioSnapshot(int date_, int priorDate_ = 0) const;
    snapshot assetAllocationSnapshot(int date_, int id_, int priorDate_ = 0) const;
    snapshot accountSnapshot(int date_, int id_, int priorDate_ = 0) const;
    snapshot symbolSnapshot(int date_, const symbol &key_, int beginDate_) const;

    double nav(const objectKeyBase &key_, int beginDate_, int endDate_) const;
    historicalNAV changeOverTime(const objectKeyBase &key_, int beginDate_, int endDate_) const;

    void detach();

private:
    QExplicitlySharedDataPointer<portfolioData> d;

    objectKeyData* data() const;
    bool save(const queries &dataSource_);
};

#endif // PORTFOLIO_H
