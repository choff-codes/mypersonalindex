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
        stats[i] = QString(statisticDisplayNames.at(i));

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

QString statistic::calculate(stat statistic, portfolio *currentPortfolio, calculations::portfolioDailyInfo *info, const int &startDate, const int &previousDay)
{
    switch(statistic)
    {
        case stat_BeginningIndexValue:
            return beginningIndexValue(currentPortfolio, previousDay);
            break;
        default:
            return QString();
    }
}

QString statistic::beginningIndexValue(portfolio *currentPortfolio, const int &previousDay)
{
    QMap<int, double>::const_iterator i = currentPortfolio->data.nav.navHistory().lowerBound(previousDay);
    if (i.key() == previousDay)
        return functions::doubleToLocalFormat(i.value());

    return QString();
}

QString statistic::beginningValue(portfolio *currentPortfolio, const int &previousDay)
{
    QMap<int, double>::const_iterator i = currentPortfolio->data.nav.totalValueHistory().lowerBound(previousDay);
    if (i.key() == previousDay)
        return functions::doubleToCurrency(i.value());

    return QString();
}

QString statistic::costBasis(calculations::portfolioDailyInfo* info)
{
    return functions::doubleToCurrency(info->costBasis);
}

QString statistic::currentIndexValue(portfolio *currentPortfolio, calculations::portfolioDailyInfo* info)
{
    QMap<int, double>::const_iterator i = currentPortfolio->data.nav.totalValueHistory().lowerBound(info->date);
    if (i.key() == info->date)
        return functions::doubleToCurrency(i.value());

    return QString();
}

QString statistic::currentValue(portfolio *currentPortfolio, calculations::portfolioDailyInfo *info)
{
    QMap<int, double>::const_iterator i = currentPortfolio->data.nav.navHistory().lowerBound(info->date);
    if (i.key() == info->date)
        return functions::doubleToLocalFormat(i.value());

    return QString();
}

QString statistic::dailyReturn(portfolio *currentPortfolio, calculations::portfolioDailyInfo* info, const int &startDate, const int &previousDay)
{
    QMap<int, double> navHistory = currentPortfolio->data.nav.navHistory();
    QMap<int, double>::const_iterator startNav = navHistory.lowerBound(previousDay);
    QMap<int, double>::const_iterator endNav = navHistory.lowerBound(info->date);
    if (startNav.key() != previousDay || endNav.key() != info->date)
        return QString();

    QList<int> dates = prices::instance().dates();
    int days = qLowerBound(dates, startDate) - qLowerBound(dates, info->date);

    if (days == 0)
        return QString();

    return functions::doubleToPercentage(100 * (pow(endNav.value() / startNav.value(), 1.0 / days) - 1));

}
