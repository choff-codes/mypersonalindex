#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QMap>
#include <QSharedData>
#include "queries.h"
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "portfolioInfo.h"

class portfolioData;
class portfolio
{
public:
    portfolio(int id_ = -1);
    portfolio(const portfolio &other_);
    ~portfolio();

    portfolio& operator=(const portfolio &other_);

    QStringList symbols() const;
    const QList<int> securityReinvestments();

    void beginNAVBatch();
    void insertNAVBatch(const queries &dataSource_);
    void beginExecutedTradesBatch();
    void insertExecutedTradesBatch(const queries &dataSource_);

    int endDate() const;

    QMap<int, security>& securities();
    QMap<int, assetAllocation> &assetAllocations();
    QMap<int, account>& accounts();
    navInfoPortfolio& navHistory();
    portfolioInfo& attributes();

private:
    QExplicitlySharedDataPointer<portfolioData> d;
};

#endif // PORTFOLIO_H
