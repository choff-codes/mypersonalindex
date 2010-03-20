#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "calculationInfo.h"
#include "prices.h"
#include "portfolio.h"

class calculations
{
public:
    calculations() {}
    virtual ~calculations() {}

    virtual void setPortfolio(const int portfolioID) { m_portfolioID = portfolioID; }
    virtual dailyInfoPortfolio* portfolioValues(const int &date);

    static double correlation(const securityPrices &price1, const securityPrices &price2, const int &startDate, const int &endDate);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

protected:
    int m_portfolioID;

    securityInfo specificSecurityValue(const security &s, const int &date);
};


#endif // CALCULATIONS_H
