#ifndef PORTFOLIODATA_H
#define PORTFOLIODATA_H

#include <QMap>
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

    portfolioData(const int &id_):
            executedTrades(executedTradeList(id_)),
            info(portfolioInfo(id_)),
            nav(navInfoPortfolio(id_))
    {}
};

#endif // PORTFOLIODATA_H
