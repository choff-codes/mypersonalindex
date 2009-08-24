#ifndef PORTFOLIOQUERIES_H
#define PORTFOLIOQUERIES_H

#include "queries.h"

class portfolioQueries : public queries
{
public:
    enum { getPortfolioAttributes_Name, getPortfolioAttributes_Dividends,
           getPortfolioAttributes_NAVStartValue, getPortfolioAttributes_CostCalc, getPortfolioAttributes_AAThreshold,
           getPortfolioAttributes_StartDate };

    queryInfo* updatePortfolio(const int &portfolio, const QString &name, const bool &dividends, const double &navStart,
        const int &costCalc, const int &aaThreshold, const QDate &startDate);
    queryInfo* getPortfolioAttributes(const int &portfolio);
    queryInfo* insertPortfolio(const QString &name, const bool &dividends, const double &navStart,
        const int &costCalc, const int &aaThreshold, const QDate &startDate);
};

#endif // PORTFOLIOQUERIES_H
