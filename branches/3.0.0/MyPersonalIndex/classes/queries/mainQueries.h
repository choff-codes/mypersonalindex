#ifndef MAINQUERIES_H
#define MAINQUERIES_H

#include "queries.h"
#include "globals.h"

class mainQueries : public queries
{
public:
    enum { getPortfolioAttributes_PortfolioID, getPortfolioAttributes_Description, getPortfolioAttributes_Dividends, getPortfolioAttributes_StartValue,
           getPortfolioAttributes_CostCalc, getPortfolioAttributes_AAThreshold, getPortfolioAttributes_AAThresholdValue,
           getPortfolioAttributes_StartDate, getPortfolioAttributes_HoldingsShowHidden, getPortfolioAttributes_HoldingsSort,
           getPortfolioAttributes_NAVSort, getPortfolioAttributes_AASort, getPortfolioAttributes_AAShowBlank,
           getPortfolioAttributes_CorrelationShowHidden, getPortfolioAttributes_AcctSort, getPortfolioAttributes_AcctShowBlank };

    enum { getSettings_DataStartDate, getSettings_LastPortfolio, getSettings_WindowX, getSettings_WindowY, getSettings_WindowHeight,
           getSettings_WindowWidth, getSettings_WindowState, getSettings_Splits };

    queryInfo* getSettings();
    queryInfo* updateSettings(const QVariant &lastPortfolio, const QSize &windowSize, const QPoint &windowLocation, const int &state);
    queryInfo* updatePortfolioAttributes(const globals::portfolio &p);
    queryInfo* getPortfolioAttributes();
    queryInfo* deletePortfolio(const int &portfolio);
};

#endif // MAINQUERIES_H
