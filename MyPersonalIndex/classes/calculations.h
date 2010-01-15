#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "calculationInfo.h"
#include "prices.h"
#include "security.h"
#include "account.h"
#include "portfolio.h"

class calculations
{
public:
    calculations(): m_portfolio(0) {}
    virtual ~calculations() {}

    virtual void setPortfolio(const portfolio *p_portfolio) { m_portfolio = p_portfolio; }
    virtual dailyInfoPortfolio* portfolioValues(const int &date);

    static double correlation(const securityPrices &price1, const securityPrices &price2, const int &startDate, const int &endDate);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

protected:
    const portfolio *m_portfolio;

    double splitRatio(const QString &symbol, const int &startDate, const int &endDate);
    securityInfo specificSecurityValue(const security &s, const int &date);
};


#endif // CALCULATIONS_H
