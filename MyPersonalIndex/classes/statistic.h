#ifndef STATISTIC_H
#define STATISTIC_H

#include <QMap>
#include <QString>
#include "statisticInfo.h"
#include "functions.h"

class statistic
{
public:
    enum stat {
        stat_BeginningIndexValue, stat_BeginningValue, stat_CostBasis, stat_CurrentIndexValue, stat_CurrentValue, stat_DailyReturn,
        stat_DailyStandardDeviation, stat_Date, stat_DaysInvested, stat_GainLoss, stat_HourlyReturn, stat_MaxPercentDown, stat_MaxPercentDownDay,
        stat_MaxPercentUp, stat_MaxPercentUpDay, stat_MaximumIndexValue, stat_MaximumIndexValueDay, stat_MaximumPortfolioValue,
        stat_MaximumPortfolioValueDay, stat_MinimumIndexValue, stat_MinimumIndexValueDay, stat_MinimumPortfolioValue, stat_MinimumPortfolioValueDay,
        stat_MonthlyReturn, stat_MonthlyStandardDeviation, stat_NetChange, stat_OverallReturn, stat_ProbabilityOfYearlyGain,
        stat_ProbabilityOfYearlyLoss, stat_TaxLiability, stat_YearlyReturn, stat_YearlyStandardDeviation, stat_WeightedExpenseRatio
    };

    static const QStringList statisticDisplayNames;

    static QString calculate(stat statistic, const statisticInfo &statInfo);
    static QMap<int, QString> statisticList();

private:
    static double returnPercent(const statisticInfo &statInfo, const double &divisor);
    static double cumulativeNormalDistribution(const statisticInfo &statInfo); 
};

#endif // STATISTIC_H
