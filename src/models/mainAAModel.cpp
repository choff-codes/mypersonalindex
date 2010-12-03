#include "mainAAModel.h"
#include <QColor>
#include "functions.h"
#include "assetAllocation.h"
#include "calculatorNAV.h"
#include "historicalNAV.h"

//enum {
//    row_Description,
//    row_CostBasis,
//    row_Value,
//    row_ValueP,
//    row_Gain,
//    row_GainP,
//    row_Target,
//    row_Variance,
//    row_RebalanceBand,
//    row_Rebalance,
//    row_Holdings
//};

const QStringList aaRow::columns = QStringList()
                                   << "Asset Class"
                                   << "Cost Basis"
                                   << "Total Value"
                                   << "% of Portfolio"
                                   << "Gain/Loss"
                                   << "% Gain Loss"
                                   << "Target"
                                   << "Variance"
                                   << "Rebalance\nBand"
                                   << "Rebalance\nAmount"
                                   << "# Holdings";

const QVariantList aaRow::columnsType = QVariantList()
                                        << QVariant(QVariant::String)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Int);


aaRow::aaRow(double nav_, const snapshot &snapshot_, const snapshot &portfolioSnapshot_, const assetAllocation &aa_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{    
    //    row_Description,
    this->values.append(aa_.description());
    //    row_CostBasis,
    this->values.append(snapshot_.costBasis);
    //    row_Value,
    this->values.append(snapshot_.totalValue);
    //    row_ValueP,
    this->values.append(functions::isZero(portfolioSnapshot_.totalValue) ? QVariant() : snapshot_.totalValue / portfolioSnapshot_.totalValue);
    //    row_Gain,
    this->values.append(snapshot_.totalValue - snapshot_.costBasis);
    //    row_GainP,
    this->values.append(nav_ - 1);
    //    row_Target,
    this->values.append(functions::isZero(aa_.target()) ? QVariant() : aa_.target());
    //    row_Variance,
    this->values.append(
        functions::isZero(portfolioSnapshot_.totalValue) || functions::isZero(aa_.target()) ?
            QVariant() :
            aa_.threshold() == assetAllocation::thresholdMethod_AA ?
                (snapshot_.totalValue / (portfolioSnapshot_.totalValue * aa_.target())) - 1 :
                (snapshot_.totalValue / portfolioSnapshot_.totalValue) - aa_.target()
        );
    //    row_RebalanceBand,
    this->values.append(functions::isZero(aa_.rebalanceBand()) ? QVariant() : aa_.rebalanceBand());
    //    row_Rebalance,
    this->values.append(
        functions::isZero(portfolioSnapshot_.totalValue) || functions::isZero(aa_.target()) ?
            QVariant() :
            -1 * portfolioSnapshot_.totalValue * ((snapshot_.totalValue / portfolioSnapshot_.totalValue) - aa_.target())
        );
    //    row_Holdings
    this->values.append(snapshot_.count);
}

QMap<int, QString> aaRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = functions::removeNewLines(columns.at(i));

    return names;
}

QList<baseRow*> aaRow::getRows(const QMap<int, assetAllocation> &assetAllocation_, int beginDate_, int endDate_, calculatorNAV calculator_,
    const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_)
{
    QList<baseRow*> returnList;

    foreach(const assetAllocation &aa, assetAllocation_)
    {
        if (aa.deleted() || aa.hidden())
            continue;

        returnList.append(getRow(aa, beginDate_, endDate_, calculator_, portfolioSnapshot_, columnSort_));
    }

    // check if any securities have an unassigned value
    snapshot unassigned = calculator_.assetAllocationSnapshot(endDate_, UNASSIGNED);
    if (unassigned.count != 0)
        returnList.append(getRow(assetAllocation(UNASSIGNED, UNASSIGNED, "(Unassigned)"), beginDate_, endDate_, calculator_, portfolioSnapshot_, columnSort_));

    return returnList;
}

baseRow* aaRow::getRow(const assetAllocation &assetAllocation_, int beginDate_, int endDate_, calculatorNAV calculator_, const snapshot &portfolioSnapshot_,
    const QList<orderBy> &columnSort_)
{
    return new aaRow(
        calculator_.nav(assetAllocation_, beginDate_, endDate_),
        calculator_.assetAllocationSnapshot(endDate_, assetAllocation_.id()), portfolioSnapshot_, assetAllocation_, columnSort_
    );
}

mainAAModel::mainAAModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QObject *parent_):
    mpiViewModelBase(rows_, viewableColumns_, parent_),
    m_portfolioSnapshot(portfolioSnapshot_),
    m_portfolioNAV(portfolioNAV_),
    m_target(0)
{
    foreach(const baseRow *r, rows_)
        m_target += r->values.at(aaRow::row_Target).toDouble();
}

QVariant mainAAModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index_.column());
    QVariant value = m_rows.at(index_.row())->values.at(column);

    if (role_ == Qt::DisplayRole)
    {
        if (value.isNull())
            return QVariant();

        switch (column)
        {
            case aaRow::row_Value:
            case aaRow::row_CostBasis:
            case aaRow::row_Gain:
            case aaRow::row_Rebalance:
                return functions::doubleToCurrency(value.toDouble());
            case aaRow::row_ValueP:
            case aaRow::row_Target:
            case aaRow::row_Variance:
            case aaRow::row_GainP:
            case aaRow::row_RebalanceBand:
                return functions::doubleToPercentage(value.toDouble());
        }

        return value;
    }

    if (role_ == Qt::TextColorRole && column == aaRow::row_Variance)
    {
        double threshold = m_rows.at(index_.row())->values.at(aaRow::row_RebalanceBand).toDouble();
        if (functions::isZero(threshold))
            return QVariant();

        return qAbs(value.toDouble()) <= threshold ? QVariant() :
            value.toDouble() > threshold ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
    }

    if (role_ == Qt::TextColorRole && column == aaRow::row_GainP)
        return functions::isZero(value.toDouble()) ?
            QVariant() :
            value.toDouble() > 0 ?
                qVariantFromValue(QColor(Qt::darkGreen)) :
                qVariantFromValue(QColor(Qt::red));

    return QVariant();
}

QVariant mainAAModel::headerData(int section_, Qt::Orientation orientation_, int role_) const
{
     if (section_ >= m_viewableColumns.count())
        return QVariant();

    if (role_ == Qt::TextAlignmentRole)
        return (int)Qt::AlignLeft | Qt::AlignVCenter;

    if (orientation_ != Qt::Horizontal || role_ != Qt::DisplayRole)
        return QVariant();

    int column = m_viewableColumns.at(section_);
    QString extra;
    switch(column)
    {
        case aaRow::row_CostBasis:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.costBasis));
            break;
        case aaRow::row_Value:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue));
            break;
        case aaRow::row_Gain:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue - m_portfolioSnapshot.costBasis));
            break;
        case aaRow::row_GainP:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_portfolioNAV - 1));
            break;
        case aaRow::row_Target:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_target));
            break;
    }

    return QString(aaRow::columns.at(column)).append(extra);
}
