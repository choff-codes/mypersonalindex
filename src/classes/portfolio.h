#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QSharedData>
#include <QString>
#include "objectBase.h"

template <class T, class V>
class QMap;
class QStringList;
class queries;
class security;
class assetAllocation;
class account;
class portfolioAttributes;
class portfolioData;
class portfolio
{
public:
    explicit portfolio(int id_ = UNASSIGNED, const QString &description_ = QString());
    portfolio(const portfolio &other_);
    ~portfolio();

    portfolio& operator=(const portfolio &other_);

    QStringList symbols() const;
    static QStringList symbols(const QMap<int, portfolio> portfolios_);
    int endDate() const;

    static int getOpenIdentity() { return --IDENTITY_COUNTER; }

    bool operator==(const portfolio &other_) const;
    bool operator!=(const portfolio &other_) const { return !(*this == other_); }

    QMap<int, security>& securities() const;
    QMap<int, assetAllocation>& assetAllocations() const;
    QMap<int, account>& accounts() const;
    portfolioAttributes& attributes() const;

    void save(const queries &dataSource_);
    static QMap<int, portfolio> save(const QMap<int, portfolio> &portfolios_, const queries &dataSource_, int *id_);
    void remove(const queries &dataSource_) const;

    void detach();

private:
    QExplicitlySharedDataPointer<portfolioData> d;
    static int IDENTITY_COUNTER;
};

#endif // PORTFOLIO_H
