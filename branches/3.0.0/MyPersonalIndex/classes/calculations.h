#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QtCore>
#include "calculationInfo.h"
#include "prices.h"
#include "portfolio.h"
#include "splits.h"

#ifndef EPSILON
#define EPSILON 0.00001
#endif

class calculations
{
public:
    calculations() {}
    virtual ~calculations() {}

    virtual void setPortfolio(const int portfolioID) { m_portfolioID = portfolioID; }
    virtual dailyInfoPortfolio* portfolioValues(const int &date);

    static double correlation(const navInfoStatistic &info1, const navInfoStatistic &info2);
    static double change(double totalValue, double previousTotalValue, double dailyActivity, double dividends, double previousNAV = 1);

protected:
    struct expensePair
    {
        expensePair(const double &p_totalValue, const double &p_expenseRatio): totalValue(p_totalValue), expenseRatio(p_expenseRatio) {}

        double totalValue;
        double expenseRatio;
    };

    int m_portfolioID;

    securityInfo securityValues(const security &s, const int &date);
};


#endif // CALCULATIONS_H
