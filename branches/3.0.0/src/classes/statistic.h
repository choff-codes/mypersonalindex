#ifndef STATISTIC_H
#define STATISTIC_H

class historicalNAV;
class statistic
{
public:
    statistic(const historicalNAV &historicalNAV_);

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
    double costBasis;
    double taxLiability;
    double dividends;
};

#endif // STATISTIC_H
