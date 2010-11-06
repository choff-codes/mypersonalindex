#include "mainAAModel.h"
#include "functions.h"
#include "calculatorNAV.h"
#include "historicalNAV.h"

//enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_Target, row_Offset, row_Holdings };
const QStringList aaRow::columns = QStringList() << "Asset Class" << "Cost Basis" << "Total Value" << "% of Portfolio"
    << "Gain/Loss" << "% Gain Loss" << "Target" << "Variance" << "Rebalance\nAmount" << "# Holdings" << "ID" << "NAV";

const QVariantList aaRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
    << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
    << QVariant(QVariant::Double) << QVariant(QVariant::Int) << QVariant(QVariant::Int) << QVariant(QVariant::Double);


aaRow::aaRow(double nav_, const snapshot &snapshot_, const snapshot &portfolioSnapshot_, const assetAllocation &aa_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{    
    //row_Description
    this->values.append(aa_.description);
    //row_CostBasis
    this->values.append(snapshot_.costBasis);
    //row_Value
    this->values.append(snapshot_.totalValue);
    //row_ValueP
    this->values.append(functions::isZero(portfolioSnapshot_.totalValue) ? QVariant() : snapshot_.totalValue / portfolioSnapshot_.totalValue);
    //row_Gain
    this->values.append(snapshot_.totalValue - snapshot_.costBasis);
    //row_GainP
    this->values.append(functions::isZero(snapshot_.costBasis) || functions::isZero(snapshot_.totalValue) ? QVariant() : (snapshot_.totalValue / snapshot_.costBasis) - 1);
    //row_Target
    this->values.append(functions::isZero(aa_.target) ? QVariant() : aa_.target);
    //row_Variance
    this->values.append(functions::isZero(portfolioSnapshot_.totalValue) || functions::isZero(aa_.target) ? QVariant() :
        aa_.threshold == assetAllocation::thresholdMethod_AA ? ((snapshot_.totalValue / (portfolioSnapshot_.totalValue * aa_.target)) - 1) :
        (snapshot_.totalValue / snapshot_.totalValue) - aa_.target);
    //row_Rebalance
    this->values.append(functions::isZero(portfolioSnapshot_.totalValue) || functions::isZero(aa_.target) ? QVariant() :
        -1 * portfolioSnapshot_.totalValue * ((snapshot_.totalValue / portfolioSnapshot_.totalValue) - aa_.target));
    //row_Holdings
    this->values.append(snapshot_.count);
    //row_ID
    this->values.append(aa_.id);
    //row_IRR
    this->values.append(nav_ - 1);
}

QMap<int, QString> aaRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = QString(columns.at(i)).replace('\n', ' ');

    names.remove(row_ID);
    return names;
}

QList<baseRow*> aaRow::getRows(const QMap<int, assetAllocation> assetAllocation_, int beginDate_, int endDate_, calculatorNAV calculator_,
    const QList<orderBy> &columnSort_, bool showHidden_, bool showUnassigned_)
{
    QList<baseRow*> returnList;

    snapshot portfolioSnapshot = calculator_.portfolioSnapshot(endDate_);

    foreach(const assetAllocation &aa, assetAllocation_)
    {
        if (aa.deleted)
            continue;

        if (aa.hide && !showHidden_)
            continue;

        returnList.append(new aaRow(calculator_.changeOverTime(aa, beginDate_, endDate_).nav(endDate_), calculator_.assetAllocationSnapshot(endDate_, aa.id), portfolioSnapshot, aa, columnSort_));
    }

    if (showUnassigned_)
    {
        assetAllocation unassignedAA;
        unassignedAA.description = "Unassigned";
        returnList.append(new aaRow(calculator_.changeOverTime(unassignedAA, beginDate_, endDate_).nav(endDate_), calculator_.assetAllocationSnapshot(endDate_, unassignedAA.id), portfolioSnapshot, unassignedAA, columnSort_));
    }

    return returnList;
}

mainAAModel::mainAAModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QTableView *parent_):
    mpiViewModelBase(rows_, viewableColumns_, parent_), m_portfolioSnapshot(portfolioSnapshot_), m_portfolioNAV(portfolioNAV_), m_target(0)
{
    foreach(const baseRow *r, rows_)
        m_target += r->values.at(aaRow::row_Target).toDouble();
}

QVariant mainAAModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index.column());
    QVariant value = m_rows.at(index.row())->values.at(column);

    if (role == Qt::DisplayRole)
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
            case aaRow::row_IRR:
                return functions::doubleToPercentage(value.toDouble());
        }

        return value;
    }

    if (role == Qt::TextColorRole && column == aaRow::row_Variance)
    {
        double threshold = m_rows.at(index.row())->values.at(aaRow::row_Target).toDouble();
        if (functions::isZero(threshold))
            return QVariant();

        return qAbs(value.toDouble()) <= threshold ? QVariant() :
            value.toDouble() > threshold ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
    }

    if (role == Qt::TextColorRole && column == aaRow::row_GainP)
        return value.toDouble() == 0 ? QVariant() :
            value.toDouble() > 0 ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));

    return QVariant();
}

QVariant mainAAModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (section >= m_viewableColumns.count())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignLeft | Qt::AlignVCenter;

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    int column = m_viewableColumns.at(section);
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
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(
                        m_portfolioSnapshot.costBasis == 0 ? 0 : (m_portfolioSnapshot.totalValue - m_portfolioSnapshot.costBasis) / m_portfolioSnapshot.costBasis)
                    );
            break;
        case aaRow::row_Target:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_target));
            break;
        case aaRow::row_IRR:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_portfolioNAV - 1));
            break;
    }

    return QString(aaRow::columns.at(column)).append(extra);
}
