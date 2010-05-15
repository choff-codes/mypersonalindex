#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <qmath.h>
#include <qnumeric.h>
#include <QHash>
#include "calculationInfo.h"
#include "prices.h"
#include "portfolio.h"
#include "splits.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

#ifndef EPSILON
#define EPSILON 0.00001
#endif

class calculations
{
public:
    calculations(const int &portfolioID): m_portfolioID(portfolioID) {}

    void clearCache() { m_cache.clear(); }

    dailyInfoPortfolio portfolioValues(const int &date, const bool &calcAveragePrices = false);
    securityInfo securityValues(const int &securityID, const int &date);
    dailyInfo aaValues(const int &date, const int &aaID);
    dailyInfo acctValues(const int &date, const int &acctID);

    navInfoStatistic changeOverTime(const objectKey &key, const int &startDate, const int &endDate, const bool &dividends);
    navInfoStatistic changeOverTime(const int &startDate, const int &endDate); // overload that uses portfolio's NAV values, much faster than recalcing

    static double correlation(const navInfoStatistic &info1, const navInfoStatistic &info2);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

private:
    typedef QPair<double /* shares */, double /* price */> sharePricePair;
    int m_portfolioID;
    QHash<int, dailyInfoPortfolio> m_cache;

    navInfoStatistic changeOverTime(const QString &symbol, const int &startDate, const int &endDate, const bool &dividends);
    dailyInfo getDailyInfoByKey(const int &date, const objectKey &key);
    QMap<int, double> avgPricePerShare(const int &calculationDate);
};


#endif // CALCULATIONS_H
