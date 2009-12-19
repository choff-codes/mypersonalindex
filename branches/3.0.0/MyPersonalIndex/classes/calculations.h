#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "globals.h"
#include "prices.h"

class calculations
{
public:

    calculations(): m_portfolio(0) {};
    virtual ~calculations() {};

    virtual void setPortfolio(const globals::myPersonalIndex *portfolio) { m_portfolio = portfolio; }
    virtual globals::portfolioDailyInfo* portfolioValues(const int &date);

protected:
    const globals::myPersonalIndex *m_portfolio;

    double splitRatio(const QString &ticker, const int &startDate, const int &endDate);
    globals::securityValue tickerValue(const globals::security &s, const int &date);
};


#endif // CALCULATIONS_H
