#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "globals.h"
#include "prices.h"

class calculations
{
public:
    struct portfolioDailyInfo
    {
        int date;
        QMap<int, double> avgPrices;
        QMap<int, globals::securityValue> tickerValue;
        double totalValue;
        double costBasis;
        double dividends;
        double commission;
        double taxLiability;

        portfolioDailyInfo(const int &p_date): date(p_date), totalValue(0), costBasis(0), dividends(0), commission(0), taxLiability(0) {}
    };

    calculations(): m_portfolio(0) {}
    virtual ~calculations() {}

    virtual void setPortfolio(const globals::myPersonalIndex *portfolio) { m_portfolio = portfolio; }
    virtual portfolioDailyInfo* portfolioValues(const int &date);

    static double correlation(const prices::securityPrices &price1, const prices::securityPrices &price2, const int &startDate, const int &endDate);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

protected:
    const globals::myPersonalIndex *m_portfolio;

    double splitRatio(const QString &ticker, const int &startDate, const int &endDate);
    globals::securityValue tickerValue(const globals::security &s, const int &date);
};


#endif // CALCULATIONS_H
