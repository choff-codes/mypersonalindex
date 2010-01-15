#ifndef CACHEDCALCULATIONS_H
#define CACHEDCALCULATIONS_H

#include <QtCore>
#include "calculations.h"

class cachedCalculations: public calculations
{
public:
    cachedCalculations(): calculations() {}
    virtual ~cachedCalculations() {}

    dailyInfoPortfolio* portfolioValues(const int &date);
    void setPortfolio(const portfolio *p_portfolio) { calculations::setPortfolio(p_portfolio); clearCache(); }
    void clearCache() { m_cache.clear(); }
    dailyInfo aaValues(const int &date, const assetAllocation &aa);
    dailyInfo acctValues(const int &date, const account &acct);

protected:
    QCache<int, dailyInfoPortfolio> m_cache;

    typedef QPair<double,double> sharePricePair;

    QMap<int, double> avgPricePerShare(const int &calculationDate);
};

#endif // CACHEDCALCULATIONS_H
