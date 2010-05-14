#ifndef CALCULATIONINFO_H
#define CALCULATIONINFO_H

#include <QMap>

struct dailyInfo
{
    int date;
    int count;
    double totalValue;
    double costBasis;
    double taxLiability;
    double dividendAmount;
    double expenseRatio;

    dailyInfo(const int &p_date): date(p_date), count(0), totalValue(0), costBasis(0), taxLiability(0), dividendAmount(0), expenseRatio(0) {}
};

struct securityInfo: public dailyInfo
{
    double shares;

    securityInfo(): dailyInfo(0), shares(0) { count = 1; }
    securityInfo(const int &p_date): dailyInfo(p_date), shares(0) { count = 1; }
};

struct dailyInfoPortfolio: public dailyInfo
{
    QMap<int, double> avgPrices;
    QMap<int, securityInfo> securitiesInfo;

    dailyInfoPortfolio(const int &p_date): dailyInfo(p_date) { count = 1; }
};

#endif // CALCULATIONINFO_H
