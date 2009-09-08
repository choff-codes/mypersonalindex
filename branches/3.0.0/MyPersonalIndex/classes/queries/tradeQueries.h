#ifndef TRADEQUERIES_H
#define TRADEQUERIES_H

#include "queries.h"

class tradeQueries : public queries
{
public:
    enum { getPortfolioAttributes_Name, getPortfolioAttributes_Dividends,
           getPortfolioAttributes_NAVStartValue, getPortfolioAttributes_CostCalc, getPortfolioAttributes_AAThreshold,
           getPortfolioAttributes_StartDate };

    queryInfo* getTrades(const int &tickerID);
};

#endif // TRADEQUERIES_H
