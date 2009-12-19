#ifndef CACHEDCALCULATIONS_H
#define CACHEDCALCULATIONS_H

#include <QtCore>
#include "calculations.h"

class cachedCalculations: public calculations
{
public:

    cachedCalculations(): calculations() {}
    virtual ~cachedCalculations() {};

    globals::portfolioDailyInfo* portfolioValues(const int &date);
    void setPortfolio(const globals::myPersonalIndex *portfolio) { m_portfolio = portfolio; clearCache(); }
    void clearCache() { m_cache.clear(); }

protected:
    QCache<int, globals::portfolioDailyInfo> m_cache;

    typedef QPair<double,double> sharePricePair;

    QMap<int, double> avgPricePerShare(const int &calculationDate);
};

#endif // CACHEDCALCULATIONS_H
