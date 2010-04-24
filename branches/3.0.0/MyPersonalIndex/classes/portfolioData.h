#ifndef PORTFOLIODATA_H
#define PORTFOLIODATA_H

#include <QtCore>
#include "executedTrade.h"
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "portfolioInfo.h"

class portfolioData
{
public:
    QMap<int, security> securities;
    QMap<int, assetAllocation> aa;
    QMap<int, account> acct;
    executedTradeList executedTrades;
    navInfoPortfolio nav;
    portfolioInfo info;
};

#endif // PORTFOLIODATA_H
