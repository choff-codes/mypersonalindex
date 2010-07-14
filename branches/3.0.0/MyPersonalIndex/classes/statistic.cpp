#include "statistic.h"

//enum {stat_BeginningIndexValue, stat_BeginningValue, stat_CostBasis, stat_CurrentIndexValue, stat_CurrentValue, stat_DailyReturn,
//      stat_DailyStandardDeviation, stat_Date, stat_DaysInvested, stat_GainLoss, stat_HourlyReturn, stat_MaxPercentDown, stat_MaxPercentDownDay,
//      stat_MaxPercentUp, stat_MaxPercentUpDay, stat_MaximumIndexValue, stat_MaximumIndexValueDay, stat_MaximumPortfolioValue,
//      stat_MaximumPortfolioValueDay, stat_MinimumIndexValue, stat_MinimumIndexValueDay, stat_MinimumPortfolioValue, stat_MinimumPortfolioValueDay,
//      stat_MonthlyReturn, stat_MonthlyStandardDeviation, stat_NetChange, stat_OverallReturn, stat_ProbabilityOfYearlyGain,
//      stat_ProbabilityOfYearlyLoss, stat_TaxLiability, stat_YearlyReturn, stat_YearlyStandardDeviation, stat_WeightedExpenseRatio } ;

const QStringList statistic::statisticDisplayNames = QStringList() << "Beginning Index Value" << "Beginning Value" << "Cost Basis"
     << "Current Index Value" <<  "Current Value" <<  "Daily Return" << "Daily Standard Deviation" << "Date" << "Days Invested"
     << "Gain Loss" << "Hourly Return" << "Max % Down" << "Max % Down Day" << "Max % Up" << "Max % Up Day" << "Maximum Index Value"
     << "Maximum Index Value Day" << "Maximum Portfolio Value" << "Maximum Portfolio Value Day" << "Minimum Index Value"
     << "Minimum Index Value Day" << "Minimum Portfolio Value" << "Minimum Portfolio Value Day" << "Monthly Return"
     << "Monthly Standard Deviation" << "Net Change" << "Overall Return" << "Probability Of Yearly Gain" << "Probability Of Yearly Loss"
     << "Tax Liability" << "Yearly Return" << "Yearly Standard Deviation" << "Weighted Expense Ratio";


QMap<int, QString> statistic::statisticList()
{
    QMap<int, QString> stats;
    for (int i = 0; i < statisticDisplayNames.count(); ++i)
        stats[i] = statisticDisplayNames.at(i);

    return stats;
}

QString statistic::calculate(stat statistic_, const statisticInfo &statisticInfo_)
{
    switch(statistic_)
    {
        case stat_BeginningIndexValue:
            return functions::doubleToLocalFormat(statisticInfo_.startNAV());
        case stat_BeginningValue:
            return functions::doubleToCurrency(statisticInfo_.startTotalValue());
        case stat_CurrentIndexValue:
            return functions::doubleToLocalFormat(statisticInfo_.endNAV());
        case stat_CurrentValue:
            return functions::doubleToCurrency(statisticInfo_.endTotalValue());
        case stat_CostBasis:
            return functions::doubleToCurrency(statisticInfo_.endInfo().costBasis);
        case stat_DailyStandardDeviation:
            return functions::doubleToPercentage(statisticInfo_.standardDeviation());
        case stat_MonthlyStandardDeviation:
            return functions::doubleToPercentage(sqrt(21) * statisticInfo_.standardDeviation());
        case stat_YearlyStandardDeviation:
            return functions::doubleToPercentage(sqrt(252) * statisticInfo_.standardDeviation());
        case stat_DaysInvested:
            return functions::doubleToLocalFormat(statisticInfo_.days(), 0);
        case stat_Date:
            return QDate::fromJulianDay(statisticInfo_.endDate()).toString(Qt::SystemLocaleShortDate);
        case stat_GainLoss:
            return functions::doubleToCurrency(statisticInfo_.endInfo().totalValue(statisticInfo_.endDate()) - statisticInfo_.endInfo().costBasis);
        case stat_DailyReturn:
            return functions::doubleToPercentage(returnPercent(statisticInfo_, 1));
        case stat_HourlyReturn:
            return functions::doubleToPercentage(returnPercent(statisticInfo_, 1.0 / 6.5));
        case stat_MonthlyReturn:
            return functions::doubleToPercentage(returnPercent(statisticInfo_, 21));
        case stat_YearlyReturn:
            return functions::doubleToPercentage(returnPercent(statisticInfo_, 252));
        case stat_NetChange:
            return functions::doubleToCurrency(statisticInfo_.endTotalValue() - statisticInfo_.startTotalValue());
        case stat_OverallReturn:
            return functions::doubleToPercentage((statisticInfo_.endNAV() / statisticInfo_.startNAV()) - 1);
        case stat_TaxLiability:
            return functions::doubleToCurrency(statisticInfo_.endInfo().taxLiability);
        case stat_MaxPercentDown:
            return functions::doubleToPercentage(statisticInfo_.maxChangeNegative());
        case stat_MaxPercentDownDay:
            return QDate::fromJulianDay(statisticInfo_.maxChangeNegativeDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaxPercentUp:
            return functions::doubleToPercentage(statisticInfo_.maxChangePositive());
        case stat_MaxPercentUpDay:
            return QDate::fromJulianDay(statisticInfo_.maxChangePositiveDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaximumIndexValue:
            return functions::doubleToLocalFormat(statisticInfo_.maxNAVValue());
        case stat_MaximumIndexValueDay:
            return QDate::fromJulianDay(statisticInfo_.maxNAVValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MinimumIndexValue:
            return functions::doubleToLocalFormat(statisticInfo_.minNAVValue());
        case stat_MinimumIndexValueDay:
            return QDate::fromJulianDay(statisticInfo_.minNAVValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MinimumPortfolioValue:
            return functions::doubleToCurrency(statisticInfo_.minTotalValue());
        case stat_MinimumPortfolioValueDay:
            return QDate::fromJulianDay(statisticInfo_.minTotalValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaximumPortfolioValue:
            return functions::doubleToCurrency(statisticInfo_.maxTotalValue());
        case stat_MaximumPortfolioValueDay:
            return QDate::fromJulianDay(statisticInfo_.maxTotalValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_ProbabilityOfYearlyGain:
            return functions::doubleToPercentage(cumulativeNormalDistribution(statisticInfo_));
        case stat_ProbabilityOfYearlyLoss:
            return functions::doubleToPercentage(1 - cumulativeNormalDistribution(statisticInfo_));
        case stat_WeightedExpenseRatio:
            return functions::doubleToPercentage(statisticInfo_.expenseRatio());
        default:
            return QString();
    }
}

double statistic::returnPercent(const statisticInfo &statisticInfo_, double divisor_)
{
    if (statisticInfo_.days() == 0 || statisticInfo_.startNAV() == 0)
        return 0;

    return pow(statisticInfo_.endNAV() / statisticInfo_.startNAV(), 1.0 / (statisticInfo_.days() / divisor_)) - 1;
}

double statistic::cumulativeNormalDistribution(const statisticInfo &statisticInfo_)
{
    if (statisticInfo_.days() < 2)
        return 0;

    //http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution
    const double b1 =  0.319381530;
    const double b2 = -0.356563782;
    const double b3 =  1.781477937;
    const double b4 = -1.821255978;
    const double b5 =  1.330274429;
    const double p  =  0.2316419;
    const double c  =  0.39894228;
    const double x = returnPercent(statisticInfo_, 252) / (sqrt(252) * statisticInfo_.standardDeviation());
    double t;

    if (x >= 0)
    {
        t = 1.0 / ( 1.0 + p * x );
        return (1.0 - c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }

    t = 1.0 / ( 1.0 - p * x );
    return ( c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
}
