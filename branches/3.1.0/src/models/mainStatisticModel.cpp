#include "mainStatisticModel.h"
#include <QStringList>
#include <qmath.h>
#include <qnumeric.h>
#include <QDate>
#include "historicalNAV.h"
#include "functions.h"
#include "statistic.h"

//enum {
//    row_ObjectType,
//    row_ID,
//    row_Description,
//    row_BeginningIndexValue,
//    row_BeginningValue,
//    row_CostBasis,
//    row_CurrentIndexValue,
//    row_CurrentValue,
//    row_DailyReturn,
//    row_DailyStandardDeviation,
//    row_Date,
//    row_DaysInvested,
//    row_Dividends,
//    row_GainLoss,
//    row_HourlyReturn,
//    row_MaxPercentDown,
//    row_MaxPercentDownDay,
//    row_MaxPercentUp,
//    row_MaxPercentUpDay,
//    row_MaximumIndexValue,
//    row_MaximumIndexValueDay,
//    row_MaximumTotalValue,
//    row_MaximumTotalValueDay,
//    row_MinimumIndexValue,
//    row_MinimumIndexValueDay,
//    row_MinimumTotalValue,
//    row_MinimumTotalValueDay,
//    row_MonthlyReturn,
//    row_MonthlyStandardDeviation,
//    row_NetChange,
//    row_OverallReturn,
//    row_ProbabilityOfYearlyGain,
//    row_ProbabilityOfYearlyLoss,
//    row_TaxLiability,
//    row_YearlyReturn,
//    row_YearlyStandardDeviation,
//    row_WeightedExpenseRatio
//};

const QStringList statisticRow::columns = QStringList()
                                          << "Type"
                                          << "ID"
                                          << "Description"
                                          << "Beginning Value"
                                          << "Cost Basis"
                                          << "Total Value"
                                          << "Daily Return"
                                          << "Daily Standard Deviation"
                                          << "Date"
                                          << "Days Invested"
                                          << "Total Dividends"
                                          << "Gain/Loss"
                                          << "Hourly Return"
                                          << "Max % Down"
                                          << "Max % Down Day"
                                          << "Max % Up"
                                          << "Max % Up Day"
                                          << "Maximum Return"
                                          << "Maximum Return Day"
                                          << "Maximum Total Value"
                                          << "Maximum Total Value Day"
                                          << "Minimum Return"
                                          << "Minimum Return Day"
                                          << "Minimum Total Value"
                                          << "Minimum Total Value Day"
                                          << "Monthly Return"
                                          << "Monthly Standard Deviation"
                                          << "Change In Value"
                                          << "Return"
                                          << "Probability Of Yearly Gain"
                                          << "Probability Of Yearly Loss"
                                          << "Tax Liability"
                                          << "Yearly Return"
                                          << "Yearly Standard Deviation"
                                          << "Weighted Expense Ratio";

const QVariantList statisticRow::columnsType = QVariantList()
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::String)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Int)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double)
                                               << QVariant(QVariant::Double);

statisticRow::statisticRow(int type_, int id_, const QString description_):
    baseRow(QList<orderBy>())
{
    //    row_ObjectType,
    values.append(type_);
    //    row_ID,
    values.append(id_);
    //    row_Description,
    values.append(description_);
}

statisticRow::statisticRow(int type_, int id_, const QString description_, const historicalNAV &historicalNav_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{
    statistic info(historicalNav_);
    //    row_ObjectType,
    values.append(type_);
    //    row_ID,
    values.append(id_);
    //    row_Description,
    values.append(description_);
    //    row_BeginningValue,
    values.append(info.beginTotalValue);
    //    row_CostBasis,
    values.append(info.costBasis);
    //    row_CurrentValue,
    values.append(info.endTotalValue);
    //    row_DailyReturn,
    values.append(indexReturn(info, 1));
    //    row_DailyStandardDeviation,
    values.append(info.standardDeviation);
    //    row_Date,
    values.append(info.endDate);
    //    row_DaysInvested,
    values.append(info.days);
    //    row_Dividends,
    values.append(info.dividends);
    //    row_GainLoss,
    values.append(info.endTotalValue - info.costBasis);
    //    row_HourlyReturn,
    values.append(indexReturn(info, 1.0 / 6.5));
    //    row_MaxPercentDown,
    values.append(info.maxChangeNegative);
    //    row_MaxPercentDownDay,
    values.append(info.maxChangeNegativeDay);
    //    row_MaxPercentUp,
    values.append(info.maxChangePositive);
    //    row_MaxPercentUpDay,
    values.append(info.maxChangePositiveDay);
    //    row_MaximumIndexValue,
    values.append(info.days <= 0 ? 0 : (info.maxNAVValue / info.beginNAV) - 1);
    //    row_MaximumIndexValueDay,
    values.append(info.maxNAVValueDay);
    //    row_MaximumTotalValue,
    values.append(info.maxTotalValue);
    //    row_MaximumTotalValueDay,
    values.append(info.maxTotalValueDay);
    //    row_MinimumIndexValue,
    values.append(info.days <= 0 ? 0 : (info.minNAVValue / info.beginNAV) - 1);
    //    row_MinimumIndexValueDay,
    values.append(info.minNAVValueDay);
    //    row_MinimumTotalValue,
    values.append(info.minTotalValue);
    //    row_MinimumTotalValueDay,
    values.append(info.minTotalValueDay);
    //    row_MonthlyReturn,
    values.append(indexReturn(info, 21));
    //    row_MonthlyStandardDeviation,
    values.append(sqrt(21) * info.standardDeviation);
    //    row_NetChange,
    values.append(info.endTotalValue - info.beginTotalValue);
    //    row_OverallReturn,
    values.append((info.endNAV / info.beginNAV) - 1);
    //    row_ProbabilityOfYearlyGain,
    double distribution = cumulativeNormalDistribution(info);
    values.append((isnan(distribution) || isinf(distribution)) ? 0 : distribution);
    //    row_ProbabilityOfYearlyLoss,
    values.append(info.days < 2 || (isnan(distribution) || isinf(distribution)) ? 0 : 1 - distribution);
    //    row_TaxLiability,
    values.append(info.taxLiability);
    //    row_YearlyReturn,
     values.append(indexReturn(info, 252));
    //    row_YearlyStandardDeviation,
    values.append(sqrt(252) * info.standardDeviation);
    //    row_WeightedExpenseRatio
    values.append(info.expenseRatio);
}

bool statisticRow::operator==(const statisticRow &other_) const
{
    return values.at(row_ObjectType).toInt() == other_.values.at(row_ObjectType).toInt()
        && values.at(row_ID).toInt() == other_.values.at(row_ID).toInt()
        && values.at(row_Description).toString() == other_.values.at(row_Description).toString();
}

QMap<int, QString> statisticRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = functions::removeNewLines(columns.at(i));

    names.remove(row_ObjectType);
    names.remove(row_ID);
    names.remove(row_Description);

    return names;
}


double statisticRow::indexReturn(const statistic &statistic_, double divisor_)
{
    if (statistic_.days <= 0 || statistic_.beginNAV == 0)
        return 0;

    return pow(statistic_.endNAV / statistic_.beginNAV, 1.0 / (statistic_.days / divisor_)) - 1;
}

double statisticRow::cumulativeNormalDistribution(const statistic &statistic_)
{
    if (statistic_.days < 2)
        return 0;

    //http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution
    const double b1 =  0.319381530;
    const double b2 = -0.356563782;
    const double b3 =  1.781477937;
    const double b4 = -1.821255978;
    const double b5 =  1.330274429;
    const double p  =  0.2316419;
    const double c  =  0.39894228;

    // x = Yearly Return / Yearly Standard Deviation
    const double x = indexReturn(statistic_, 252) / (sqrt(252) * statistic_.standardDeviation);
    double t;

    if (x >= 0)
    {
        t = 1.0 / ( 1.0 + p * x );
        return (1.0 - c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }

    t = 1.0 / ( 1.0 - p * x );
    return ( c * exp( -x * x / 2.0 ) * t * ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
}

mainStatisticModel::mainStatisticModel(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QObject *parent_ ):
    mpiViewModelBase(rows_, viewableColumns_, true, parent_)
{
}

QVariant mainStatisticModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ != Qt::DisplayRole)
        return QVariant();

    int column = m_viewableColumns.at(index_.row());
    QVariant value = m_rows.at(index_.column())->values.at(column);

    switch (column)
    {
        case statisticRow::row_BeginningValue:
        case statisticRow::row_CostBasis:
        case statisticRow::row_GainLoss:
        case statisticRow::row_NetChange:
        case statisticRow::row_TaxLiability:
        case statisticRow::row_MaximumTotalValue:
        case statisticRow::row_MinimumTotalValue:
        case statisticRow::row_CurrentValue:
        case statisticRow::row_Dividends:
            return functions::doubleToCurrency(value.toDouble());
        case statisticRow::row_MaximumIndexValue:
        case statisticRow::row_MinimumIndexValue:
        case statisticRow::row_DailyReturn:
        case statisticRow::row_DailyStandardDeviation:
        case statisticRow::row_HourlyReturn:
        case statisticRow::row_MonthlyReturn:
        case statisticRow::row_MonthlyStandardDeviation:
        case statisticRow::row_OverallReturn:
        case statisticRow::row_ProbabilityOfYearlyGain:
        case statisticRow::row_ProbabilityOfYearlyLoss:
        case statisticRow::row_MaxPercentDown:
        case statisticRow::row_MaxPercentUp:
        case statisticRow::row_YearlyReturn:
        case statisticRow::row_YearlyStandardDeviation:
            return functions::doubleToPercentage(value.toDouble());
        case statisticRow::row_WeightedExpenseRatio:
            return functions::doubleToPercentage(value.toDouble(), 4);
        case statisticRow::row_Date:
        case statisticRow::row_MaxPercentDownDay:
        case statisticRow::row_MaxPercentUpDay:
        case statisticRow::row_MaximumIndexValueDay:
        case statisticRow::row_MaximumTotalValueDay:
        case statisticRow::row_MinimumIndexValueDay:
        case statisticRow::row_MinimumTotalValueDay:
            return QDate::fromJulianDay(value.toInt()).toString(Qt::SystemLocaleShortDate);
        case statisticRow::row_DaysInvested:
            return value.toInt();
    };

    return QVariant();
}

QVariant mainStatisticModel::headerData(int section_, Qt::Orientation orientation_, int role_) const
{
    if (role_ != Qt::DisplayRole)
        return QVariant();

    if (orientation_ == Qt::Vertical)
        return statisticRow::columns.at(m_viewableColumns.at(section_));

    if (orientation_ == Qt::Horizontal)
        return m_rows.at(section_)->values.at(statisticRow::row_Description);

    return QVariant();
}

void mainStatisticModel::add(statisticRow *row_)
{
    beginInsertColumns(QModelIndex(), m_rows.count(), m_rows.count());
    m_rows.append(row_);
    endInsertColumns();

    sortRows();
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1));
}

void mainStatisticModel::remove(const statisticRow &row_)
{
    int x = -1;
    for(int i = 0; i < m_rows.count(); ++i)
        if (*static_cast<statisticRow*>(m_rows.at(i)) == row_)
        {
            x = i;
            break;
        }

    if (x == -1)
        return;

    beginRemoveColumns(QModelIndex(), x, x);
    m_rows.removeAt(x);
    endRemoveColumns();
}
