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

void statistic::saveSelectedStats(const int &portfolioID, const QList<int> &stats)
{
    QVariantList portfolio, statID, sequence;
    for(int i = 0; i < stats.count(); ++i)
    {
        portfolio.append(portfolioID);
        statID.append(stats.at(i));
        sequence.append(i);
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_PortfolioID), portfolio);
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_StatID), statID);
    tableValues.insert(queries::statMappingColumns.at(queries::statMappingColumns_Sequence), sequence);

    queries::deletePortfolioItems(queries::table_StatMapping, portfolioID);
    if (!statID.isEmpty())
        queries::executeTableUpdate(queries::table_StatMapping, tableValues);
}

QString statistic::calculate(stat statistic, const statisticInfo &statInfo)
{
    switch(statistic)
    {
        case stat_BeginningIndexValue:
            return functions::doubleToLocalFormat(statInfo.startNAV());
        case stat_BeginningValue:
            return functions::doubleToCurrency(statInfo.startTotalValue());
        case stat_CurrentIndexValue:
            return functions::doubleToLocalFormat(statInfo.endNAV());
        case stat_CurrentValue:
            return functions::doubleToCurrency(statInfo.endTotalValue());
        case stat_CostBasis:
            return functions::doubleToCurrency(statInfo.endInfo()->costBasis);
        case stat_DailyStandardDeviation:
            return functions::doubleToPercentage(statInfo.standardDeviation());
        case stat_MonthlyStandardDeviation:
            return functions::doubleToPercentage(sqrt(21) * statInfo.standardDeviation());
        case stat_YearlyStandardDeviation:
            return functions::doubleToPercentage(sqrt(252) * statInfo.standardDeviation());
        case stat_DaysInvested:
            return functions::doubleToLocalFormat(statInfo.days(), 0);
        case stat_Date:
            return QDate::fromJulianDay(statInfo.endDate()).toString(Qt::SystemLocaleShortDate);
        case stat_GainLoss:
            return functions::doubleToCurrency(statInfo.endInfo()->totalValue - statInfo.endInfo()->costBasis);
        case stat_DailyReturn:
            return functions::doubleToPercentage(returnPercent(statInfo, 1));
        case stat_HourlyReturn:
            return functions::doubleToPercentage(returnPercent(statInfo, 1.0 / 6.5));
        case stat_MonthlyReturn:
            return functions::doubleToPercentage(returnPercent(statInfo, 21));
        case stat_YearlyReturn:
            return functions::doubleToPercentage(returnPercent(statInfo, 252));
        case stat_NetChange:
            return functions::doubleToCurrency(statInfo.endTotalValue() - statInfo.startTotalValue());
        case stat_OverallReturn:
            return functions::doubleToPercentage(100 * ((statInfo.endNAV() / statInfo.startNAV()) - 1));
        case stat_TaxLiability:
            return functions::doubleToCurrency(statInfo.endInfo()->taxLiability);
        case stat_MaxPercentDown:
            return functions::doubleToPercentage(statInfo.maxChangeNegative());
        case stat_MaxPercentDownDay:
            return QDate::fromJulianDay(statInfo.maxChangeNegativeDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaxPercentUp:
            return functions::doubleToPercentage(statInfo.maxChangePositive());
        case stat_MaxPercentUpDay:
            return QDate::fromJulianDay(statInfo.maxChangePositiveDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaximumIndexValue:
            return functions::doubleToLocalFormat(statInfo.maxNAVValue());
        case stat_MaximumIndexValueDay:
            return QDate::fromJulianDay(statInfo.maxNAVValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MinimumIndexValue:
            return functions::doubleToLocalFormat(statInfo.minNAVValue());
        case stat_MinimumIndexValueDay:
            return QDate::fromJulianDay(statInfo.minNAVValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MinimumPortfolioValue:
            return functions::doubleToCurrency(statInfo.minTotalValue());
        case stat_MinimumPortfolioValueDay:
            return QDate::fromJulianDay(statInfo.minTotalValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_MaximumPortfolioValue:
            return functions::doubleToCurrency(statInfo.maxTotalValue());
        case stat_MaximumPortfolioValueDay:
            return QDate::fromJulianDay(statInfo.maxTotalValueDay()).toString(Qt::SystemLocaleShortDate);
        case stat_ProbabilityOfYearlyGain:
            return functions::doubleToPercentage(100 * cumulativeNormalDistribution(statInfo));
        case stat_ProbabilityOfYearlyLoss:
            return functions::doubleToPercentage(100 - (100 * cumulativeNormalDistribution(statInfo)));
        case stat_WeightedExpenseRatio:
            return functions::doubleToPercentage(weightedExpenseRatio(statInfo));
        default:
            return QString();
    }
}

double statistic::returnPercent(const statisticInfo &statInfo, const double &divisor)
{
    if (statInfo.days() == 0 || statInfo.startNAV() == 0)
        return 0;

    return 100 * (pow(statInfo.endNAV() / statInfo.startNAV(), 1.0 / (statInfo.days() / divisor)) - 1);
}

double statistic::cumulativeNormalDistribution(const statisticInfo &statInfo)
{
    if (statInfo.days() < 2)
        return 0;

    //http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution
    const double b1 =  0.319381530;
    const double b2 = -0.356563782;
    const double b3 =  1.781477937;
    const double b4 = -1.821255978;
    const double b5 =  1.330274429;
    const double p  =  0.2316419;
    const double c  =  0.39894228;
    const double x = returnPercent(statInfo, 252) / (sqrt(252) * statInfo.standardDeviation());
    double t;

    if (x >= 0)
    {
        t = 1.0 / ( 1.0 + p * x );
        return (1.0 - c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }

    t = 1.0 / ( 1.0 - p * x );
    return ( c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
}

double statistic::weightedExpenseRatio(const statisticInfo &statInfo)
{
    double er = 0;
    const QMap<int, securityInfo> securities = statInfo.endInfo()->securitiesInfo;
    double totalValue = statInfo.endTotalValue();

    foreach(const security &s, statInfo.securities())
        if (s.expense > 0)
            er += s.expense * (securities.value(s.id).totalValue / totalValue);

    return er;
}
