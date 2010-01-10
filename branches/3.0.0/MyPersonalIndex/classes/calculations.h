#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "prices.h"
#include "security.h"
#include "account.h"
#include "portfolio.h"

class calculations
{
public:
    struct securityValue
    {
        double totalValue;
        double shares;
        double costBasis;
        double dividendAmount;
        double commission;
        double taxLiability;

        securityValue(): totalValue(0), shares(0), costBasis(0), dividendAmount(0), commission(0), taxLiability(0) {}
    };
    
    struct portfolioDailyInfo
    {
        int date;
        QMap<int, double> avgPrices;
        QMap<int, securityValue> securityValues;
        double totalValue;
        double costBasis;
        double dividends;
        double commission;
        double taxLiability;

        portfolioDailyInfo(const int &p_date): date(p_date), totalValue(0), costBasis(0), dividends(0), commission(0), taxLiability(0) {}
    };

    calculations(): m_portfolio(0) {}
    virtual ~calculations() {}

    virtual void setPortfolio(const portfolio *p_portfolio) { m_portfolio = p_portfolio; }
    virtual portfolioDailyInfo* portfolioValues(const int &date);

    static double correlation(const prices::securityPrices &price1, const prices::securityPrices &price2, const int &startDate, const int &endDate);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

protected:
    const portfolio *m_portfolio;

    double splitRatio(const QString &symbol, const int &startDate, const int &endDate);
    securityValue specificSecurityValue(const security &s, const int &date);
};


#endif // CALCULATIONS_H
