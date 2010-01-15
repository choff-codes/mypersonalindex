#ifndef CALCULATIONINFO_H
#define CALCULATIONINFO_H

#include <QtCore>

struct dailyInfo
{
    int date;
    int count;
    double totalValue;
    double costBasis;
    double taxLiability;

    dailyInfo(const int &p_date): date(p_date), count(0), totalValue(0), costBasis(0), taxLiability(0) {}
};

struct securityInfo
{
    double totalValue;
    double shares;
    double costBasis;
    double dividendAmount;
    double commission;
    double taxLiability;

    securityInfo(): totalValue(0), shares(0), costBasis(0), dividendAmount(0), commission(0), taxLiability(0) {}
};

struct dailyInfoPortfolio
{
    int date;
    QMap<int, double> avgPrices;
    QMap<int, securityInfo> securitiesInfo;
    double totalValue;
    double costBasis;
    double dividends;
    double commission;
    double taxLiability;

    dailyInfoPortfolio(const int &p_date): date(p_date), totalValue(0), costBasis(0), dividends(0), commission(0), taxLiability(0) {}
};

#endif // CALCULATIONINFO_H
