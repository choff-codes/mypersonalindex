#ifndef PORTFOLIO_P_H
#define PORTFOLIO_P_H

#include <QMap>
#include <QDate>
#include "calculatorNAV.h"
#include "security.h"
#include "account.h"
#include "assetAllocation.h"

class portfolioData: public objectKeyData
{
public:
    calculatorNAV calculator;
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;
    int startDate;

    portfolioData():
        startDate(QDate::currentDate().toJulianDay())
    {}
};

#endif // PORTFOLIO_P_H
