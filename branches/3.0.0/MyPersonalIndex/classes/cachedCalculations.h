#ifndef CACHEDCALCULATIONS_H
#define CACHEDCALCULATIONS_H

#include <QString>
#include <QMap>
#include <QHash>
#include "calculations.h"
#include "objectKey.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class cachedCalculations: public calculations
{
public:
    cachedCalculations(): calculations() {}
    virtual ~cachedCalculations() { clearCache(); }

    void setPortfolio(const int &portfolioID) { calculations::setPortfolio(portfolioID); clearCache(); }
    void clearCache() { qDeleteAll(m_cache); m_cache.clear(); }

    dailyInfoPortfolio* portfolioValues(const int &date, const bool &calcAveragePrices = false);
    dailyInfo aaValues(const int &date, const int &aaID);
    dailyInfo acctValues(const int &date, const int &acctID);

    navInfoStatistic changeOverTime(const objectKey &key, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic portfolioChange(const int &startDate, const int &endDate);

protected:
    QHash<int, dailyInfoPortfolio*> m_cache;

    QMap<int, double> avgPricePerShare(const int &calculationDate);

private:
    struct sharePricePair
    {
        sharePricePair(const double &p_shares, const double &p_price): shares(p_shares), price(p_price) {}

        double shares;
        double price;
    };

    navInfoStatistic changeOverTime(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends);
    dailyInfo getDailyInfoByKey(const int &date, const objectKey &key);

};

#endif // CACHEDCALCULATIONS_H
