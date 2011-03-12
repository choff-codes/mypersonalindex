#ifndef MAINSTATISTICMODEL_H
#define MAINSTATISTICMODEL_H

#include "mpiViewModelBase.h"

class historicalNAV;
class statistic;
class statisticRow: public baseRow
{
public:
    enum {
        row_PortfolioID,
        row_ObjectType,
        row_ID,
        row_Description,
        row_BeginningValue,
        row_CostBasis,
        row_CurrentValue,
        row_DailyReturn,
        row_DailyStandardDeviation,
        row_Date,
        row_DaysInvested,
        row_Dividends,
        row_GainLoss,
        row_HourlyReturn,
        row_MaxPercentDown,
        row_MaxPercentDownDay,
        row_MaxPercentUp,
        row_MaxPercentUpDay,
        row_MaximumIndexValue,
        row_MaximumIndexValueDay,
        row_MaximumTotalValue,
        row_MaximumTotalValueDay,
        row_MinimumIndexValue,
        row_MinimumIndexValueDay,
        row_MinimumTotalValue,
        row_MinimumTotalValueDay,
        row_MonthlyReturn,
        row_MonthlyStandardDeviation,
        row_NetChange,
        row_OverallReturn,
        row_ProbabilityOfYearlyGain,
        row_ProbabilityOfYearlyLoss,
        row_TaxLiability,
        row_YearlyReturn,
        row_YearlyStandardDeviation,
        row_WeightedExpenseRatio
    };

    static const QStringList columns;
    static const QVariantList columnsType;

    statisticRow(int portfolioID_, int type_, int id_, const QString description_);
    statisticRow(int portfolioID_, int type_, int id_, const QString description_,
        const historicalNAV &historicalNav_, const QList<orderBy> &columnSort_);

    bool operator==(const statisticRow &other_) const;
    bool operator!=(const statisticRow &other_) const { return !(*this == other_); }
    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();

    double indexReturn(const statistic &statistic_, double divisor_);
    double cumulativeNormalDistribution(const statistic &statistic_);
};

class mainStatisticModel: public mpiViewModelBase
{
public:
    mainStatisticModel(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QObject *parent_ = 0);

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int section, Qt::Orientation orientation_, int role_) const;

    void add(statisticRow *row_);
    void remove(const statisticRow &row_);
};

#endif // MAINSTATISTICMODEL_H
