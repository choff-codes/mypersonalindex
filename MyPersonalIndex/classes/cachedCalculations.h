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
    dailyInfo aaValues(const int &date, const int &aaID);
    dailyInfo acctValues(const int &date, const int &acctID);

protected:
    QCache<int, dailyInfoPortfolio> m_cache;

    QMap<int, double> avgPricePerShare(const int &calculationDate);

private:
    struct sharePricePair
    {
        sharePricePair(const double &p_shares, const double &p_price): shares(p_shares), price(p_price) {}

        double shares;
        double price;
    };
};

#endif // CACHEDCALCULATIONS_H
