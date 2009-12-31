#include "mainAcctModel.h"

//enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_TaxRate, row_TaxLiability, row_Net, row_Holdings };
const QStringList acctRow::columns = QStringList() << "Account" << "Cost Basis" << "Total Value" << "% of Portfolio" << "Gain/Loss" << "% Gain/Loss"
    << "Tax Rate" << "Tax Liability" << "After Tax Value" << "# of Holdings";

const QVariantList acctRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Int);

acctRow::acctRow* acctRow::getAcctRow(const calculations::portfolioDailyInfo *info, const cachedCalculations::dailyInfo &acctInfo, const account &acct, const QString &sort)
{
    acctRow *row = new acctRow(sort);

    //row_Description
    row->values.append(acct.description);
    //row_CostBasis
    row->values.append(acctInfo.costBasis);
    //row_Value
    row->values.append(acctInfo.totalValue);
    //row_ValueP
    row->values.append(info->totalValue == 0 ? QVariant() : 100.0 * acctInfo.totalValue / info->totalValue);
    //row_Gain
    row->values.append(acctInfo.totalValue - acctInfo.costBasis);
    //row_GainP
    row->values.append(acctInfo.costBasis == 0 ? QVariant() : ((acctInfo.totalValue / acctInfo.costBasis) - 1) * 100);
    //row_TaxRate
    row->values.append(acct.taxRate < 0 ? QVariant() : acct.taxRate);
    //row_TaxLiability
    row->values.append(acctInfo.taxLiability);
    //row_Net
    row->values.append(acctInfo.totalValue - acctInfo.taxLiability);
    //row_Holdings
    row->values.append(acctInfo.count);

    return row;
}

QMap<int, QString> acctRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = QString(columns.at(i)).replace('\n', ' ');

    return names;
}

QVariant mainAcctModel::data(const QModelIndex &index, int role) const
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
            case acctRow::row_CostBasis:
            case acctRow::row_Value:
            case acctRow::row_Gain:
            case acctRow::row_TaxLiability:
            case acctRow::row_Net:
                return functions::doubleToCurrency(m_rows.at(index.row())->values.at(column).toDouble());
            case acctRow::row_ValueP:
            case acctRow::row_GainP:
            case acctRow::row_TaxRate:
                return functions::doubleToPercentage(m_rows.at(index.row())->values.at(column).toDouble());
        }

        return m_rows.at(index.row())->values.at(column);
    }

    if (role == Qt::TextColorRole && column == acctRow::row_GainP)
    {
        double value = m_rows.at(index.row())->values.at(column).toDouble();
        return value == 0 ? QVariant() :
            value > 0 ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
    }

    return QVariant();
}

QVariant mainAcctModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        case acctRow::row_CostBasis:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_costBasis));
            break;
        case acctRow::row_Value:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue));
            break;
        case acctRow::row_Gain:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_costBasis));
            break;
        case acctRow::row_Net:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_taxLiability));
            break;
        case acctRow::row_TaxLiability:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_taxLiability));
            break;
    }

    return QString(acctRow::columns.at(column)).append(extra);
}
