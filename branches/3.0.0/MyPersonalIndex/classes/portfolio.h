#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QMap>
#include <QSharedData>
#include "queries.h"
#include "historicalNAV.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "portfolioAttributes.h"

class portfolioData;
class portfolio
{
public:
    portfolio(int id_ = -1);
    portfolio(const portfolio &other_);
    ~portfolio();

    portfolio& operator=(const portfolio &other_);

    QStringList symbols() const;

    int endDate() const;

    QMap<int, security>& securities();
    QMap<int, assetAllocation>& assetAllocations();
    QMap<int, account>& accounts();
    portfolioAttributes& attributes();

private:
    QExplicitlySharedDataPointer<portfolioData> d;
};

#endif // PORTFOLIO_H
