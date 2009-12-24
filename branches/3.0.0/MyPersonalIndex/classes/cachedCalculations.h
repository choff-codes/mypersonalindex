#ifndef CACHEDCALCULATIONS_H
#define CACHEDCALCULATIONS_H

#include <QtCore>
#include "calculations.h"

class cachedCalculations: public calculations
{
public:
    struct dailyInfo
    {
        int date;
        int count;
        double totalValue;
        double costBasis;
        double taxLiability;

        dailyInfo(const int &p_date): date(p_date), count(0), totalValue(0), costBasis(0), taxLiability(0) {}
    };


    cachedCalculations(): calculations() {}
    virtual ~cachedCalculations() {}

    portfolioDailyInfo* portfolioValues(const int &date);
    void setPortfolio(const globals::myPersonalIndex *portfolio) { m_portfolio = portfolio; clearCache(); }
    void clearCache() { m_cache.clear(); }
    dailyInfo aaValues(const int &date, const globals::assetAllocation &aa);
    dailyInfo acctValues(const int &date, const globals::account &acct);

protected:
    QCache<int, portfolioDailyInfo> m_cache;

    typedef QPair<double,double> sharePricePair;

    QMap<int, double> avgPricePerShare(const int &calculationDate);
};

#endif // CACHEDCALCULATIONS_H
