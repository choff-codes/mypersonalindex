#include "mainAAModel.h"

//enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_Target, row_Offset, row_Holdings };
const QStringList aaRow::columns = QStringList() << "Asset Allocation" << "Cost Basis" << "Total Value" << "% of Portfolio"
    << "Gain/Loss" << "% Gain Loss" << "Target" << "Offset" << "Offset Amount" << "# Holdings" << "ID";

const QVariantList aaRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
    << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
    << QVariant(QVariant::Double) << QVariant(QVariant::Int) << QVariant(QVariant::Int);


aaRow::aaRow(const calculations::portfolioDailyInfo *info, const cachedCalculations::dailyInfo &aaInfo, portfolioInfo::thesholdMethod method,
    const assetAllocation &aa, const QString &sort): baseRow(sort)
{
    //row_Description
    this->values.append(aa.description);
    //row_CostBasis
    this->values.append(aaInfo.costBasis);
    //row_Value
    this->values.append(aaInfo.totalValue);
    //row_ValueP
    this->values.append(info->totalValue == 0 ? QVariant() : 100.0 * aaInfo.totalValue / info->totalValue);
    //row_Gain
    this->values.append(aaInfo.totalValue - aaInfo.costBasis);
    //row_GainP
    this->values.append(aaInfo.costBasis == 0 ? QVariant() : ((aaInfo.totalValue / aaInfo.costBasis) - 1) * 100);
    //row_Target
    this->values.append(aa.target < 0 ? QVariant() : aa.target);
    //row_Offset
    this->values.append(info->totalValue == 0 || aa.target < 0 ? QVariant() : 100.0 *
        (method == portfolioInfo::theshold_AA ? ((aaInfo.totalValue / (info->totalValue * aa.target / 100.0)) - 1) :
        (aaInfo.totalValue / info->totalValue) - (aa.target / 100.0)));
    //row_OffsetAmount
    this->values.append(info->totalValue == 0 || aa.target < 0 ? QVariant() : info->totalValue * ((aaInfo.totalValue / info->totalValue) - (aa.target / 100.0)));
    //row_Holdings
    this->values.append(aaInfo.count);
    //row_ID
    this->values.append(aa.id);
}

QMap<int, QString> aaRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = QString(columns.at(i)).replace('\n', ' ');

    names.remove(row_ID);
    return names;
}

QVariant mainAAModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index.column());

    if (role == Qt::DisplayRole)
    {
        if (m_rows.at(index.row())->values.at(column).isNull())
            return QVariant();

        switch (column)
        {
            case aaRow::row_Value:
            case aaRow::row_CostBasis:
            case aaRow::row_Gain:
            case aaRow::row_OffsetAmount:
                return functions::doubleToCurrency(m_rows.at(index.row())->values.at(column).toDouble());
            case aaRow::row_ValueP:
            case aaRow::row_Target:
            case aaRow::row_Offset:
            case aaRow::row_GainP:
                return functions::doubleToPercentage(m_rows.at(index.row())->values.at(column).toDouble());
        }

        return m_rows.at(index.row())->values.at(column);
    }

    if (role == Qt::TextColorRole && column == aaRow::row_Offset)
    {
        double value = m_rows.at(index.row())->values.at(column).toDouble();
        return qAbs(value) <= m_threshold ? QVariant() :
            value > m_threshold ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
    }

    if (role == Qt::TextColorRole && column == aaRow::row_GainP)
    {
        double value = m_rows.at(index.row())->values.at(column).toDouble();
        return value == 0 ? QVariant() :
            value > 0 ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
    }

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
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_costBasis));
            break;
        case aaRow::row_Value:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue));
            break;
        case aaRow::row_Gain:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_costBasis));
            break;
    }

    return QString(aaRow::columns.at(column)).append(extra);
}
