#ifndef STATISTIC_H
#define STATISTIC_H

#include <QtGui>
#include "statisticInfo.h"
#include "portfolio.h"
#include "calculations.h"
#include "functions.h"

class statistic
{
public:
    enum outputFormat { format_Currency, format_Percentage, format_Decimal, format_Integer, format_ShortDate, format_LongDate, format_None };
    enum statVariables { stat_Portfolio, stat_PortfolioName, stat_StartDate, stat_EndDate, stat_PreviousDay, stat_TotalValue };

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

    static QString returnPercent(const statisticInfo &statInfo, const double &divisor);

    static QMap<int, QString> statisticList();
    static void saveSelectedStats(const int &portfolioID, const QList<int> &stats);
};

#endif // STATISTIC_H
