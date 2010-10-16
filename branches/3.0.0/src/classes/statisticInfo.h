#ifndef STATISTICINFO_H
#define STATISTICINFO_H

#include <qmath.h>
#include <qnumeric.h>
#include "historicalNAV.h"
#include "tradeDateCalendar.h"

class statisticInfo
{
public:
    statisticInfo(const historicalNAV &historicalNAV_, double beginNAV_ = 1);

    historicalNAV navHistory;
    int beginDate;
    int endDate;
    double beginNAV;
    double endNAV;
    double beginTotalValue;
    double endTotalValue;
    int days;
    double standardDeviation;
    double maxChangePositive;
    int maxChangePositiveDay;
    double maxChangeNegative;
    int maxChangeNegativeDay;
    double minNAVValue;
    int minNAVValueDay;
    double maxNAVValue;
    int maxNAVValueDay;
    double minTotalValue;
    int minTotalValueDay;
    double maxTotalValue;
    int maxTotalValueDay;
    double expenseRatio;
};

#endif // STATISTICINFO_H
