#ifndef MAINQUERIES_H
#define MAINQUERIES_H

#include "queries.h"

class mainQueries : public queries
{
public:
    enum { getPortfolioAttributes_ID, getPortfolioAttributes_Name, getPortfolioAttributes_Dividends, getPortfolioAttributes_HoldingsShowHidden,
           getPortfolioAttributes_NAVSort, getPortfolioAttributes_NAVStartValue, getPortfolioAttributes_CostCalc, getPortfolioAttributes_AAThreshold,
           getPortfolioAttributes_StartDate, getPortfolioAttributes_HoldingsSort, getPortfolioAttributes_AASort, getPortfolioAttributes_AAShowBlank,
           getPortfolioAttributes_CorrelationShowHidden, getPortfolioAttributes_AcctSort, getPortfolioAttributes_AcctShowBlank };

    enum { getSettings_DataStartDate, getSettings_LastPortfolio, getSettings_WindowX, getSettings_WindowY, getSettings_WindowHeight,
           getSettings_WindowWidth, getSettings_WindowState, getSettings_Splits };

    queryInfo* getSettings();
    queryInfo* updatePortfolioAttributes(const int &portfolio, const bool &holdingsShowHidden, const bool &navSort,
            const bool &showAABlank, const QString &holdingsSort, const QString &aaSort,
            const bool &correlationShowHidden, const bool &showAcctBlank, const QString &acctSort);
    queryInfo* getPortfolioAttributes(const int &portfolio);
};

#endif // MAINQUERIES_H
