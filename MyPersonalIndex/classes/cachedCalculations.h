#ifndef CACHEDCALCULATIONS_H
#define CACHEDCALCULATIONS_H

#include <QtCore>
#include "calculations.h"

class cachedCalculations: public calculations
{
public:
    cachedCalculations(): calculations() {}
    virtual ~cachedCalculations() { clearCache(); }

    dailyInfoPortfolio* portfolioValues(const int &date, const bool &calcAveragePrices = false);
    void setPortfolio(const int &portfolioID) { calculations::setPortfolio(portfolioID); clearCache(); }
    void clearCache() { qDeleteAll(m_cache); m_cache.clear(); }
    dailyInfo aaValues(const int &date, const int &aaID);
    dailyInfo acctValues(const int &date, const int &acctID);
    navInfoStatistic securityChange(const int &securityID, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic symbolChange(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic aaChange(const int &aaID, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic acctChange(const int &acctID, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic portfolioChange(const int &startDate, const int &endDate, const bool &dividends);
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

    enum changeType { changeType_Security, changeType_AA, changeType_Acct, changeType_Portfolio };

    dailyInfo getDailyInfoByType(const int &date, const int &id, const changeType &type);
    navInfoStatistic changeOverTime(const changeType &type, const int &id, const int &startDate, const int &endDate, const bool &dividends);
};

#endif // CACHEDCALCULATIONS_H
